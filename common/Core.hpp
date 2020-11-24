#pragma once
#include <fstream>
#include <memory>
#include <string>

#include "common/ILoggable.hpp"

namespace std { // for overload matching when converting a formatter argument to
                // stirng
std::string to_string(bool b);
std::string to_string(char c);
std::string to_string(const char *str);
std::string to_string(const std::string &str);
std::string to_string(std::string &&str);
std::string to_string(const rgl::ILoggable &loggable);
std::string to_string(rgl::ILoggable &&loggable);
} // namespace std

#include "common/Formatter.hpp"
#include "common/Logger.hpp"
