#ifndef SimpleConnect_h
#define SimpleConnect_h

#include <Arduino.h>
#include "LoggerSupport.h"
#include "SimpleConnectInternal.h"
#include "ConnectionManager.h"
#include "WebInterfaceManager.h"
#include "InternalWebServer.h"

/**
 * @brief Simplified WiFi + WebServer management library for ESP32.
 *
 * Automatically handles STA + fallback AP, mDNS, and basic HTTP server setup.
 */
class SimpleConnectClass
{
public:
    /**
     * @brief Constructor.
     */
    SimpleConnectClass();

    /**
     * @brief Set a custom logger implementation.
     *
     * @param logger Pointer to a LoggerInterface.
     */
    void setLogger(LoggerSupport::LogHandler logger);

    /**
     * @brief Initialize WiFi connection and web server.
     *
     * @param hostname Desired mDNS/hostname.
     * @param ssid     WiFi SSID (optional).
     * @param passphrase WiFi password (optional).
     * @param IP       Static IP (optional).
     * @param gateway  Static gateway (optional).
     * @param subnet   Static subnet mask (optional).
     */
    void init(const char *hostname,
              const char *ssid = "",
              const char *passphrase = "",
              IPAddress IP = IPAddress(),
              IPAddress gateway = IPAddress(),
              IPAddress subnet = IPAddress());

    /**
     * @brief Main loop method. Must be called regularly.
     */
    void loop();

    /**
     * @brief Returns true if WiFi is in AP mode
     */
    bool isAPStarted();

    /**
     * @brief Returns true if WiFi is connected to a network
     */
    bool isConnected();

    /**
     * @brief Resets all saved values. You still need to reboot the microcontroller to disconnect to current WiFi
     */
    void forget();

    /**
     * @brief The web server that can be used to handle HTTP communication
     */
    InternalWebServer &webServer;

private:
    ConnectionManager connectionManager;
    WebInterfaceManager webInterfaceManager;
};

extern SimpleConnectClass SimpleConnect;

#endif