#pragma once
#include "cli/BuildTarget.hpp"
#include "cli/CliArguments.hpp"
#include "common/ILoggable.hpp"

namespace rgl {
class Context : public ILoggable {
public:
  std::string m_projectName;
  std::string m_projectDir;
  BuildTarget m_target;
  CliArguments m_cliArguments;

  static Context &getInstance() {
    static Context instance;
    return instance;
  }

  std::string toString() const override {
    return Formatter("Context<name: {}, target: {}>", m_projectName, m_target)
        .toString();
  }

private:
  Context() {}
};
}; // namespace rgl
