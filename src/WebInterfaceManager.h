#ifndef WebInterfaceManager_h
#define WebInterfaceManager_h

#include <ArduinoJson.h>
#include "InternalWebServer.h"
#include "ConnectionManager.h"
#include "LoggerSupport.h"
#include "SimpleConnectInternal.h"
#include "settings_html.h"

/**
 * @brief Handles all web server routes and wrappers around the ESP32 WebServer.
 */
class WebInterfaceManager
{
public:
    /**
     * @brief Initializes the web server and internal routes.
     *
     * @param connManager The connection manager of this library.
     */
    void begin(ConnectionManager *connManager);

    /**
     * @brief The HTTP server
     */
    InternalWebServer webServer{80};

private:
    ConnectionManager *connectionManager = nullptr;

    bool isReservedPath(const String &path);
};

#endif
