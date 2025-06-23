#include "WebInterfaceManager.h"

void WebInterfaceManager::begin(ConnectionManager *connManager)
{
    connectionManager = connManager;

    // Endpoint serving the WiFi settings web interface
    webServer.WebServer::on("/wifi/settings", HTTP_GET, [this]()
                            { webServer.send(200, "text/html", settings_html); });

    // API endpoint returning a JSON describing the current status
    webServer.WebServer::on("/wifi/api/status", HTTP_GET, [this]()
                            {
        JsonDocument doc = connectionManager->getStatusJSON();
        String json;
        serializeJson(doc, json);
        webServer.send(200, "application/json", json); });

    // API endpoint returning a JSON listing all available networks
    webServer.WebServer::on("/wifi/api/scan", HTTP_GET, [this]()
                            {
        JsonDocument doc = connectionManager->getAvailableNetworksJson();
        String json;
        serializeJson(doc, json);
        webServer.send(200, "application/json", json); });

    // API endpoint receiving network parameters final users are requesting to use to connect to a network
    webServer.WebServer::on("/wifi/api/connect", HTTP_POST, [this]()
                            {
        if (webServer.args() == 0) {
            webServer.send(400, "application/json", "{\"error\":\"Missing body\"}");
            return;
        }

        // Parse incoming JSON
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, webServer.arg(0));
        if (error) {
            webServer.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            return;
        }

        const char *ssid = doc["ssid"] | "";
        const char *passphrase = doc["passphrase"] | "";
        IPAddress ip, gateway, subnet;
        ip.fromString(doc["ip"] | "");
        gateway.fromString(doc["gateway"] | "");
        subnet.fromString(doc["subnet"] | "");

        if (strlen(ssid) == 0) {
            webServer.send(400, "application/json", "{\"error\":\"SSID is required\"}");
            return;
        }

        connectionManager->connectNew(ssid, passphrase, ip, gateway, subnet);
        webServer.send(200, "application/json", "{\"status\":\"Connection attempt started\"}"); });

    // API endpoint receiving no data, triggering the forget routine
    webServer.WebServer::on("/wifi/api/forget", HTTP_POST, [this]()
                            {
        connectionManager->forget();
        webServer.send(200, "application/json", "{\"status\":\"WiFi credentials forgotten\"}"); });

    // Binds a onNotFound handler to the original webServer, so that it uses the user provided handler unless in AP mode
    webServer.WebServer::onNotFound([this]()
                                    {
        if (connectionManager && connectionManager->isAPStarted()) {
            webServer.sendHeader("Location", "http://" + connectionManager->APIP().toString() + "/wifi/settings");
            webServer.send(302, "text/plain", "");
        } else if (webServer.getUserNotFoundHandler()) {
            webServer.getUserNotFoundHandler()(); // Calls user's custom handler
        } else {
            webServer.send(404, "text/html", "NOT FOUND");
        } });

    // Begin the webServer
    webServer.WebServer::begin();
    LoggerSupport::log("Web server started");
}

// Prevent users from overwriting internal routes
bool WebInterfaceManager::isReservedPath(const String &path)
{
    for (const auto &r : RESERVED_PATHS)
    {
        if (path == r)
            return true;
    }
    return false;
}
