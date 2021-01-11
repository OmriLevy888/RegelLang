#pragma once
#include "common/Core.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <variant>

namespace rgl {
class ITokenGenerator : public ILoggable {
public:
  virtual TokenValuePair getNext() = 0;

  virtual std::shared_ptr<SourceProject> getSourceProject() const = 0;
};
}; // namespace rgl
