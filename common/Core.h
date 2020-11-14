#pragma once
#include <fstream>
#include <string>
#include <memory>

#include "common/ILoggable.h"

namespace std {  // for overload matching when converting a formatter argument to stirng
    std::string to_string(bool b);
    std::string to_string(char c);
    std::string to_string(const char *str);
    std::string to_string(const std::string& str);
    std::string to_string(std::string&& str);
    std::string to_string(const rgl::ILoggable& loggable);
    std::string to_string(rgl::ILoggable&& loggable);
}

#include "common/Logger.h"
#include "common/Formatter.h"