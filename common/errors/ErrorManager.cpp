#include "common/errors/ErrorManager.hpp"

namespace rgl {
std::queue<ErrorTypes> ErrorManager::s_errors;
std::queue<WarningTypes> ErrorManager::s_warnings;

ErrorTypes ErrorManager::getErrorType() {
  auto ret = ErrorManager::s_errors.front();
  ErrorManager::s_errors.pop();
  return ret;
}

WarningTypes ErrorManager::getWarningType() {
  auto ret = ErrorManager::s_warnings.front();
  ErrorManager::s_warnings.pop();
  return ret;
}
}; // namespace rgl
