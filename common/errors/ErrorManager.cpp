#include "common/errors/ErrorManager.hpp"
#include "common/utils/Demangle.hpp"
#include <cxxabi.h>
#include <execinfo.h>
#include <sstream>

// start of executable
extern char __executable_start;

namespace rgl {
ErrorTypes ErrorManager::getErrorType() {
  auto ret = get().m_errors.front();
  get().m_errors.pop();
  return ret;
}

WarningTypes ErrorManager::getWarningType() {
  auto ret = get().m_warnings.front();
  get().m_warnings.pop();
  return ret;
}

std::string execCommand(const char *cmd) {
  std::array<char, 4096> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe{popen(cmd, "r"), pclose};
  if (!pipe) {
    return "FAILED TO RUN " + std::string(cmd);
  }
  while (nullptr != std::fgets(buffer.data(), buffer.size(), pipe.get())) {
    result += buffer.data();
  }
  return result;
}

void ErrorManager::printStackTrace() {
#ifdef RGL_TESTS
  return;
#else
  const size_t backtraceMax = 64;
  void *stackFrames[backtraceMax];
  char **strings = nullptr;
  size_t countFrames = ::backtrace(stackFrames, backtraceMax);
  strings = backtrace_symbols(stackFrames, countFrames);

  if (nullptr != strings) {
    for (size_t i = 1; i < countFrames;
         i++) { // start from 1 to skip the printStackTrace function
      const std::string rawTrace = strings[i];

      const size_t binaryPathLen = rawTrace.find('(');
      const auto binaryPath = rawTrace.substr(0, binaryPathLen);

      const size_t mangledNameStart = binaryPathLen + 1;
      const size_t mangledSymbolLen = rawTrace.find('+') - mangledNameStart;
      const auto demangledSymbol =
          demangle(rawTrace.substr(mangledNameStart, mangledSymbolLen));
      if ("__libc_start_main" == demangledSymbol ||
          "_start" == demangledSymbol) {
        continue;
      }

      const size_t addrStart = rawTrace.find('[') + 1;
      const size_t addrEnd = rawTrace.size() - addrStart - 1;
      const auto addrStr = rawTrace.substr(addrStart, addrEnd);
      size_t addr;
      std::stringstream addrStrToNum;
      addrStrToNum << std::hex << addrStr;
      addrStrToNum >> addr;
      addr -= (size_t)&__executable_start;
      std::stringstream addrNumToStr;
      addrNumToStr << std::hex << addr;
      std::string hexRelativeAddrStr{addrNumToStr.str()};

      std::string addr2lineCmd =
          "addr2line -e " + binaryPath + " " + hexRelativeAddrStr;
      std::cerr << execCommand(addr2lineCmd.c_str()) << '\t' << demangledSymbol
                << std::endl;
    }
  }
  std::free(strings);
#endif
}
}; // namespace rgl
