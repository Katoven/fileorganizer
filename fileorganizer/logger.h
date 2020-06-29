#pragma once

#include <string>

enum struct LoggerType {
    Message,
    Trace,
    Warning,
    Error,
};

class Logger
{
public:
    virtual void Log(const LoggerType type, const std::string& message) = 0;
};