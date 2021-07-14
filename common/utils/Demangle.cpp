#include "common/utils/Demangle.hpp"
#include <cxxabi.h>

namespace rgl {
struct handle {
  char *p;
  handle(char *ptr) : p(ptr) {}
  ~handle() { std::free(p); }
};

std::string demangle(const std::string &mangled) {
  int status = -4;
  handle result =
      abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, &status);
  return (0 == status) ? result.p : mangled;
}
}; // namespace rgl
