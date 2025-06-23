#ifndef InternalWebServer_h
#define InternalWebServer_h

#include <WebServer.h>
#include "LoggerSupport.h"
#include "SimpleConnectInternal.h"

class InternalWebServer : public WebServer
{
public:
    // Constructor passthroughs
    InternalWebServer(int port = 80) : WebServer(port) {}

    // Override basic .on
    void on(const String &uri, THandlerFunction handler)
    {
        if (isReservedPath(uri))
        {
            LoggerSupport::log("Attempted to override reserved path: " + uri, LoggerSupport::WARN);
            return;
        }
        WebServer::on(uri.c_str(), handler);
    }

    // Override with method
    void on(const String &uri, HTTPMethod method, THandlerFunction handler)
    {
        if (isReservedPath(uri))
        {
            LoggerSupport::log("Attempted to override reserved path: " + uri, LoggerSupport::WARN);
            return;
        }
        WebServer::on(uri.c_str(), method, handler);
    }

    // Override .onNotFound to just store handler
    void onNotFound(THandlerFunction fn)
    {
        userNotFoundHandler = fn;
        WebServer::onNotFound(fn);
    }

    // Allow other parts of the library to access stored user handler
    THandlerFunction getUserNotFoundHandler() const
    {
        return userNotFoundHandler;
    }

private:
    THandlerFunction userNotFoundHandler = nullptr;

    bool isReservedPath(const String &path)
    {
        for (const auto &r : RESERVED_PATHS)
        {
            if (path == r)
                return true;
        }
        return false;
    }
};

#endif
