#ifndef ConnectionManager_h
#define ConnectionManager_h

#include <WiFi.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "LoggerSupport.h"
/**
 * @brief Handles connection logic for WiFi, including tracking state and triggering appropriate routines.
 */
class ConnectionManager
{
public:
    ConnectionManager();

    /**
     * @brief Initializes connection parameters.
     */
    void begin(const char *hostname,
               const char *ssid,
               const char *passphrase,
               IPAddress IP,
               IPAddress gateway,
               IPAddress subnet);

    /**
     * @brief Call this periodically to check connection status and reconnect if needed.
     */
    void checkConnection();

    /**
     * @brief Returns true if an Access Point has been started.
     */
    bool isAPStarted() const;

    /**
     * @brief Returns the access point IP address.
     */
    IPAddress APIP() const;

    /**
     * @brief Returns true if the system is connected to WiFi
     */
    bool isConnected() const;

    /**
     * @brief Attempts to connect to the specified network at runtime.
     */
    void connectNew(
        const char *ssid,
        const char *passphrase,
        IPAddress IP,
        IPAddress gateway,
        IPAddress subnet);

    /**
     * @brief Clears all stored WiFi credentials and disconnects from the current network.
     */
    void forget();

    /**
     * @brief Get a list of available WiFi networks in JSON format
     */
    JsonDocument getAvailableNetworksJson();

    /**
     * @brief Provides a JSON string describing the current WiFi status
     */
    JsonDocument getStatusJSON();

    /**
     * @brief Saved WiFi settings to memory
     */
    void persistWiFiSettingsToMemory();

    /**
     * @brief Reads and loads WiFi settings from memory
     */
    void retrieveWiFiSettingsFromMemory();

private:
    bool apStarted = false;
    bool connected = false;

    unsigned long previousCheckMillis = millis();
    unsigned long previousOfflineMillis = millis();
    unsigned long interval = 10000;

    const char *hostname = "";
    const char *ssid = "";
    const char *passphrase = "";

    IPAddress IP;
    IPAddress gateway;
    IPAddress subnet;
    DNSServer dnsServer;

    JsonDocument lastScanResult = JsonArray();
    unsigned long lastScanTime = 0;
    bool scanInProgress = false;

    void connect();
    void startAP();
    void stopAP();
    void successfulConnectionRoutine();
};

#endif
