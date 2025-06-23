#include "LoggerSupport.h"

LoggerSupport::LogHandler LoggerSupport::logHandler = nullptr;

// Log a message with the logger, or falls back to Serial
void LoggerSupport::log(const String &msg, int level)
{
    if (logHandler)
    {
        logHandler(msg, level);
    }
    else
    {
        if (level < 0 || level > 7)
            level = INFO;

        // Fallback to Serial
        static const char *levelNames[] = {
            "DEBUG", "INFO", "NOTICE", "WARN", "ERROR", "CRITICAL", "ALERT", "EMERG"};

        Serial.print("[");
        Serial.print(levelNames[level]);
        Serial.print("] ");
        Serial.println(msg);
    }
}

// Allow users to set their own logger
void LoggerSupport::setLogger(LogHandler handler)
{
    logHandler = handler;
}