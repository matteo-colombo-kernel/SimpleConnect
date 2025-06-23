#include "ConnectionManager.h"

ConnectionManager::ConnectionManager() {}

// Sets up initial connection configuration
void ConnectionManager::begin(const char *hostname,
                              const char *ssid,
                              const char *passphrase,
                              IPAddress IP,
                              IPAddress gateway,
                              IPAddress subnet)
{
    // Reload settings from memory
    retrieveWiFiSettingsFromMemory();

    // Always save the hostname at begin
    this->hostname = hostname;

    // If a SSID is provided at begin, use that - may overwrite settings from memory
    // Added instructions to avoid leaks
    if (ssid && strlen(ssid) > 0)
    {
        if (this->ssid)
        {
            free((void *)this->ssid);
            this->ssid = nullptr;
        }
        this->ssid = strdup(ssid);
    }

    // If a passphrase is provided at begin, use that - may overwrite settings from memory
    // Added instructions to avoid leaks
    if (passphrase && strlen(passphrase) > 0)
    {
        if (this->passphrase)
        {
            free((void *)this->passphrase);
            this->passphrase = nullptr;
        }
        this->passphrase = strdup(passphrase);
    }

    // If an IP address is provided at begin, use that - may overwrite settings from memory
    if (IP != IPAddress(0, 0, 0, 0))
        this->IP = IP;

    // If a gateway IP address is provided at begin, use that - may overwrite settings from memory
    if (gateway != IPAddress(0, 0, 0, 0))
        this->gateway = gateway;

    // If a subnet mask is provided at begin, use that - may overwrite settings from memory
    if (subnet != IPAddress(0, 0, 0, 0))
        this->subnet = subnet;

    // Save the new values, which may have changed considering the begin overwrites
    persistWiFiSettingsToMemory();

    // Set WiFi to both Access Point and Station mode
    WiFi.mode(WIFI_AP_STA);

    // If a SSID is set, connect
    if (this->ssid && strlen(this->ssid) > 0)
    {
        connect();
    }
    // Else, directly start the Access Point
    else
    {
        startAP();
    }
}

// Call this in loop to manage reconnections and AP fallback
void ConnectionManager::checkConnection()
{
    // Makes the DNS server work when active, ie when the AP is redirecting all traffic to the captive portal
    if (apStarted)
        dnsServer.processNextRequest();

    // Check connectivity every 15 seconds
    unsigned long now = millis();
    if (now - previousCheckMillis > 15000)
    {
        // Save the current iteration for the next check
        previousCheckMillis = now;

        // If no connection is available
        if (WiFi.status() != WL_CONNECTED)
        {
            // Set the connected flag
            connected = false;

            // Try to connect only if an SSID is in memory
            if (ssid && strlen(ssid) > 0)
            {
                LoggerSupport::log("No connection available.", LoggerSupport::WARN);
                connect();
            }

            // Anyway, as we are lacking connection, turn on the AP
            startAP();
        }
        else // in case connection is present:
        {
            // If the status is flagged not connected, that means this is the first iteration after gaining connection;
            // start the successfully connected routine
            if (!connected)
            {
                successfulConnectionRoutine();
            }

            // In any case, turn off the AP
            stopAP();
        }
    }
}

// Configures WiFi and attempts to connect
void ConnectionManager::connect()
{
    // If available, set the hostname
    if (hostname && strlen(hostname) > 0)
    {
        WiFi.setHostname(hostname);
    }

    // If given, use custom IP settings
    bool hasStaticIP = IP != IPAddress() && gateway != IPAddress() && subnet != IPAddress();
    if (hasStaticIP)
        WiFi.config(IP, gateway, subnet);

    LoggerSupport::log("Starting WiFi connection attempt...");

    // Begin the connection
    WiFi.begin(ssid, passphrase);
}

// Called on successful connection, handles mDNS and logs info
void ConnectionManager::successfulConnectionRoutine()
{
    // On successful connection, stop the AP and the previously set mDNS server
    stopAP();
    MDNS.end();

    LoggerSupport::log("WiFi connected. SSID: " + WiFi.SSID() +
                           " | Gateway: " + WiFi.gatewayIP().toString() +
                           " | IP: " + WiFi.localIP().toString() +
                           " | Hostname: " + WiFi.getHostname(),
                       LoggerSupport::INFO);

    // If hostname is set, start the mDSN service
    if (hostname && strlen(hostname) > 0)
    {
        if (MDNS.begin(hostname))
        {
            MDNS.addService("http", "tcp", 80);
            LoggerSupport::log("mDNS responder started");
        }
        else
        {
            LoggerSupport::log("Error setting up MDNS responder!", LoggerSupport::ERROR);
        }
    }

    // Set this flag
    connected = true;
}

// Starts the fallback Access Point mode
void ConnectionManager::startAP()
{
    // If already started, do not proceed
    if (apStarted)
        return;

    // Set this flag
    apStarted = true;

    LoggerSupport::log("Starting the access point...");

    // Turn off mDNS
    MDNS.end();

    // Initialize AP with default SSID, but set a custom one if hostname is set
    String apName = "CONFIG_AP";
    if (hostname && strlen(hostname) > 0)
    {
        apName = String(hostname) + "_AP";
    }

    // Set hardcoded IP values for the AP
    IPAddress apIP(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);

    // Configure and start the AP
    WiFi.softAPConfig(apIP, apIP, subnet);
    WiFi.softAP(apName);

    // Set the DNS server in DHCP responses, so that connected devices poll this AP also for DNS resolution
    esp_netif_t *ap_netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    if (ap_netif)
    {
        esp_netif_dns_info_t dns;
        dns.ip.u_addr.ip4.addr = apIP;
        dns.ip.type = IPADDR_TYPE_V4;
        esp_netif_set_dns_info(ap_netif, ESP_NETIF_DNS_MAIN, &dns);
    }

    LoggerSupport::log("WiFi Access Point Started. SSID: " + apName +
                           " | Gateway: " + WiFi.softAPIP().toString() +
                           ((hostname && strlen(hostname) > 0) ? " | Hostname: " + String(hostname) : ""),
                       LoggerSupport::INFO);

    // Start the DNS server
    dnsServer.start(53, "*", WiFi.softAPIP());

    LoggerSupport::log("DNS Server for captive portal redirection started.");
}

// Stops the Access Point
void ConnectionManager::stopAP()
{
    // If already stopped, do not proceed
    if (!apStarted)
        return;

    // Set this flag
    apStarted = false;

    // Stop the DNS server
    dnsServer.stop();
    LoggerSupport::log("DNS Server for captive portal redirection stopped.");

    // Stop the AP
    delay(200);
    WiFi.softAPdisconnect();
    LoggerSupport::log("WiFi Access Point Stopped");
}

// Return the flag of AP status
bool ConnectionManager::isAPStarted() const
{
    return apStarted;
}

// return the IP address of the AP
IPAddress ConnectionManager::APIP() const
{
    return WiFi.softAPIP();
}

// Return the flag of connected status
bool ConnectionManager::isConnected() const
{
    return connected;
}

// This function is called when user sets a new connection
void ConnectionManager::connectNew(
    const char *ssid,
    const char *passphrase,
    IPAddress IP,
    IPAddress gateway,
    IPAddress subnet)
{
    // Free old strings if needed
    if (this->ssid && strlen(this->ssid) > 0)
    {
        free((void *)this->ssid);
        this->ssid = nullptr;
    }

    if (this->passphrase && strlen(this->passphrase) > 0)
    {
        free((void *)this->passphrase);
        this->passphrase = nullptr;
    }

    // Duplicate new strings

    if (ssid && strlen(ssid) > 0)
        this->ssid = strdup(ssid);

    if (passphrase && strlen(passphrase) > 0)
        this->passphrase = strdup(passphrase);

    // Save IP settings
    this->IP = IP;
    this->gateway = gateway;
    this->subnet = subnet;

    // Persist the new values to memory
    persistWiFiSettingsToMemory();

    // Start connection attempt
    connect();
}

// This function is called when a user requests to clear WiFi credentials
void ConnectionManager::forget()
{
    LoggerSupport::log("Forgetting stored WiFi credentials...");

    // Free and nullify SSID and passphrase
    if (ssid && strlen(ssid) > 0)
    {
        free((void *)ssid);
        ssid = "";
    }

    if (passphrase && strlen(passphrase) > 0)
    {
        free((void *)passphrase);
        passphrase = "";
    }

    // Clear static IP settings
    IP = IPAddress();
    gateway = IPAddress();
    subnet = IPAddress();

    // Persist the empty values to memory
    persistWiFiSettingsToMemory();

    // Disconnect from current WiFi and set this flag
    delay(200);
    WiFi.disconnect(true);
    connected = false;

    // Start the AP
    startAP();
}

// Used by the web interface to get a list of available networks
JsonDocument ConnectionManager::getAvailableNetworksJson()
{
    JsonDocument doc;

    // Check scan result
    int n = WiFi.scanComplete();
    LoggerSupport::log("Scan status: " + String(n), LoggerSupport::DEBUG);

    if (n == -2)
    {
        // Scan not started

        JsonArray lastNetworks = lastScanResult["networks"].as<JsonArray>();

        // If a scan is in progress, or if scan is not stale and non empty
        if ((scanInProgress || (((millis() - lastScanTime) <= 30000) && !lastNetworks.isNull() && lastNetworks.size() > 0)))
        {
            LoggerSupport::log("Using scan from memory", LoggerSupport::DEBUG);
        }
        else
        {
            LoggerSupport::log("Starting async WiFi scan...");
            WiFi.scanNetworks(true);
            scanInProgress = true;
        }
    }
    else if (n == -1)
    {
        // Scan ongoing
        LoggerSupport::log("WiFi scan still in progress...", LoggerSupport::DEBUG);
    }
    else if (n >= 0)
    {
        // Scan completed
        LoggerSupport::log("WiFi scan completed with " + String(min(n, 10)) + " networks");
        scanInProgress = false;
        lastScanResult.clear();
        JsonArray networks = lastScanResult["networks"].to<JsonArray>();

        for (int i = 0; i < min(n, 10); ++i)
        {
            int rssi = WiFi.RSSI(i);
            int strength;
            if (rssi <= -67)
                strength = 1;
            else if (rssi > -67 && rssi <= -60)
                strength = 2;
            else if (rssi > -60 && rssi <= -50)
                strength = 3;
            else
                strength = 4;

            JsonObject network = networks.add<JsonObject>();
            network["ssid"] = WiFi.SSID(i);
            network["strength"] = strength;
            network["secure"] = (WiFi.encryptionType(i) != WIFI_AUTH_OPEN);
        }

        lastScanTime = millis();
        WiFi.scanDelete(); // Free memory
    }

    // Return cached result if recent or if scanning
    if (((millis() - lastScanTime) <= 30000 || n < 0) && lastScanResult["networks"].is<JsonArray>())
    {
        LoggerSupport::log("Returning last successful scan", LoggerSupport::DEBUG);
        return lastScanResult;
    }

    // Otherwise, return empty result
    doc["networks"].to<JsonArray>(); // Empty array
    return doc;
}

// Used by web interface to get the status of the connection
JsonDocument ConnectionManager::getStatusJSON()
{
    JsonDocument doc;

    // Create the "network" object
    JsonObject network = doc["network"].to<JsonObject>();
    network["hostname"] = hostname ? hostname : "";
    network["ssid"] = ssid ? ssid : "";
    int strength = 0;

    // If WiFi is connected, return its properties...
    if (WiFi.isConnected())
    {
        network["ip"] = WiFi.localIP().toString();
        network["gateway"] = WiFi.gatewayIP().toString();
        network["subnet"] = WiFi.subnetMask().toString();

        // Map RSSI to a 1 to 4 scale
        int rssi = WiFi.RSSI();
        if (rssi <= -67)
            strength = 1;
        else if (rssi > -67 && rssi <= -60)
            strength = 2;
        else if (rssi > -60 && rssi <= -50)
            strength = 3;
        else
            strength = 4;
    }
    else // else, return empty values
    {
        network["ip"] = "";
        network["gateway"] = "";
        network["subnet"] = "";
    }

    network["strength"] = strength;

    // Create the "ap" object
    JsonObject ap = doc["ap"].to<JsonObject>();
    ap["active"] = apStarted;
    ap["ssid"] = (hostname && strlen(hostname) > 0) ? String(hostname) + "_AP" : "";

    // If AP is started, return its properties...
    if (apStarted)
    {
        ap["ip"] = WiFi.softAPIP().toString();
        ap["subnet"] = "255.255.255.0";
    }
    else // else, return empty values
    {
        ap["ip"] = "";
        ap["subnet"] = "";
    }

    return doc;
}

// This is used to persist WiFi settings between restarts
void ConnectionManager::persistWiFiSettingsToMemory()
{
    // Initialize LittleFS
    if (!LittleFS.begin(true))
    {
        LoggerSupport::log("LittleFS Mount Failed: cannot persist WiFi settings to memory", LoggerSupport::ERROR);
        return;
    }

    // Write to the WiFi settings file
    File file = LittleFS.open("/wifi_settings.json", FILE_WRITE);
    if (!file)
    {
        LoggerSupport::log("LittleFS Failed to open file for writing: cannot persist WiFi settings to memory", LoggerSupport::ERROR);
        return;
    }

    // Initialize an object and set all properties
    JsonDocument doc;
    JsonObject obj = doc.to<JsonObject>();

    obj["ssid"] = ssid ? ssid : "";
    obj["passphrase"] = passphrase ? passphrase : "";
    obj["ip"] = IP.toString();
    obj["gateway"] = gateway.toString();
    obj["subnet"] = subnet.toString();

    if (serializeJson(doc, file) == 0)
    {
        LoggerSupport::log("LittleFS Failed to write JSON: cannot persist WiFi settings to memory", LoggerSupport::ERROR);
    }

    file.close();
}

// This is used to retrieve WiFi settings from memory and setting the related variables
void ConnectionManager::retrieveWiFiSettingsFromMemory()
{
    // Initialize LittleFS
    if (!LittleFS.begin(true))
    {
        LoggerSupport::log("LittleFS Mount Failed: cannot retrieve WiFi settings from memory", LoggerSupport::ERROR);
        return;
    }

    // Read from the WiFi settings file
    File file = LittleFS.open("/wifi_settings.json", FILE_READ);
    if (!file)
    {
        LoggerSupport::log("LittleFS Failed to open file for reading: cannot retrieve WiFi settings from memory", LoggerSupport::ERROR);
        return;
    }

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, file);
    if (err)
    {
        LoggerSupport::log("LittleFS Deserialize failed: cannot retrieve WiFi settings from memory", LoggerSupport::ERROR);
        LoggerSupport::log(err.c_str(), LoggerSupport::ERROR);
        file.close();
        return;
    }

    JsonObject obj = doc.as<JsonObject>();

    // Save the values from LittleFS to variables, considering strict type checks and fallbacks
    if (ssid && strlen(ssid) > 0)
    { // Free previously allocated memory
        free((void *)ssid);
        ssid = nullptr;
    }
    // Duplicate into new heap memory
    ssid = strdup(obj["ssid"].is<const char *>() ? obj["ssid"].as<const char *>() : "");

    if (passphrase && strlen(passphrase) > 0)
    { // Free previously allocated memory
        free((void *)passphrase);
        passphrase = nullptr;
    }
    // Duplicate into new heap memory
    passphrase = strdup(obj["passphrase"].is<const char *>() ? obj["passphrase"].as<const char *>() : "");

    String ipStr = obj["ip"].is<const char *>() ? obj["ip"].as<const char *>() : "";
    String gatewayStr = obj["gateway"].is<const char *>() ? obj["gateway"].as<const char *>() : "";
    String subnetStr = obj["subnet"].is<const char *>() ? obj["subnet"].as<const char *>() : "";

    if (!IP.fromString(ipStr) || !gateway.fromString(gatewayStr) || !subnet.fromString(subnetStr))
    {
        LoggerSupport::log("Failed to parse IP, gateway, or subnet from memory", LoggerSupport::ERROR);
        file.close();
        return;
    }

    file.close();
    return;
}
