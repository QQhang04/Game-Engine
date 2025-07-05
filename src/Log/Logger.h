#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <string>

enum LogType {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_COLLISION
};

struct LogEntry {
    LogType type;
    std::string message;
};

class Logger {
    public:
        static std::vector<LogEntry> messages;
        static void Log(const std::string& message);
        static void Err(const std::string& message);
        static void LogEvent(const std::string& message);
};

#endif