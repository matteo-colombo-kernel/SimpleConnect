#include "SimpleConnect.h"

/**
 * Constructor + Expose the webserver for the user to register endpoints and other customization
 */
SimpleConnectClass::SimpleConnectClass()
    : webServer(webInterfaceManager.webServer)
{
}

/**
 * Sets the logger instance.
 */
void SimpleConnectClass::setLogger(LoggerSupport::LogHandler logger)
{
    LoggerSupport::setLogger(logger);
}

/**
 * Initializes connection and server.
 */
void SimpleConnectClass::init(const char *hostname,
                              const char *ssid,
                              const char *passphrase,
                              IPAddress IP,
                              IPAddress gateway,
                              IPAddress subnet)
{
    connectionManager.begin(hostname, ssid, passphrase, IP, gateway, subnet);
    webInterfaceManager.begin(&connectionManager);
}

/**
 * Main loop.
 */
void SimpleConnectClass::loop()
{
    connectionManager.checkConnection();
    webInterfaceManager.webServer.handleClient();
}

bool SimpleConnectClass::isAPStarted()
{
    return connectionManager.isAPStarted();
}

bool SimpleConnectClass::isConnected()
{
    return connectionManager.isConnected();
}

void SimpleConnectClass::forget()
{
    return connectionManager.forget();
}

// Instantiate the global SimpleConnect object
SimpleConnectClass SimpleConnect;