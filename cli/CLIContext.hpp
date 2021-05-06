#pragma once
#include "cli/BuildTarget.hpp"
#include "cli/CLIArguments.hpp"
#include "common/ILoggable.hpp"

namespace rgl {
class CLIContext : public ILoggable {
public:
  std::string m_projectName;
  std::string m_projectDir;
  BuildTarget m_target;
  CLIArguments m_cliArguments;

  static CLIContext &getInstance() {
    static CLIContext instance;
    return instance;
  }

  std::string toString() const override {
    return Formatter("Context<name: {}, target: {}>", m_projectName, m_target)
        .toString();
  }

private:
  CLIContext() {}
};
}; // namespace rgl
