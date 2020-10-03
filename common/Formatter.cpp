#include "common/Formatter.h"

namespace std {  // for overload matching when converting a formatter argument to stirng
    std::string to_string(const std::string& str) { return str; }
    std::string to_string(std::string&& str) { return str; }
    std::string to_string(const rgl::ILoggable& loggable) { return loggable.toString(); }
    std::string to_string(rgl::ILoggable&& loggable) { return loggable.toString(); }
}