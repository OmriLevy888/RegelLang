#include "Logger.h"
#include <cstring>

namespace rgl {
LogLevel Logger::s_logLevel;

bool Logger::s_prefixType;
bool Logger::s_prefixDate;


bool Logger::doPrefixes(LogLevel level, const std::string& type) {
    if (static_cast<uint8_t>(s_logLevel) > static_cast<uint8_t>(level)) return false;
    if (s_prefixDate || s_prefixType) {
        std::cerr << "[";
        if (s_prefixDate) {
            time_t now = time(0);
            char* dt = ctime(&now);
            dt[std::strlen(dt) - 1] = 0;  // remove the \n at the end of the time string
            std::cerr << dt;
        }
        if (s_prefixDate & s_prefixType) std::cerr << " - ";
        if (s_prefixType) std::cerr << type;
        std::cerr << "] ";
    }
    return true;
}
void Logger::log(const std::string& message, LogLevel level, const std::string& type) {
    if (!doPrefixes(level, type)) return;
    std::cerr << message << std::endl;
}
void Logger::log(std::string&& message, LogLevel level, const std::string& type) {
    if (!doPrefixes(level, type)) return;
    std::cerr << message << std::endl;
}
}  // namespace rgl