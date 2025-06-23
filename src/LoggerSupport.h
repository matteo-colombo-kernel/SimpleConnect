#ifndef LoggerSupport_h
#define LoggerSupport_h

#include <Arduino.h>

class LoggerSupport
{
public:
  /** The level of the log entry */
  enum LogLevel
  {
    DEBUG,    // 0
    INFO,     // 1
    NOTICE,   // 2
    WARN,     // 3
    ERROR,    // 4
    CRITICAL, // 5
    ALERT,    // 6
    EMERG     // 7
  };

  /**
   * A handler for log calls.
   */
  typedef void (*LogHandler)(const String &message, int level);

  /**
   * Logs a message using the provided log handler or falls back to Serial.
   */
  static void log(const String &msg, int level = INFO);

  /**
   * Sets a custom log handler.
   * If null, will fall back to Serial.
   */
  static void setLogger(LogHandler handler);

private:
  static LogHandler logHandler;
};

#endif