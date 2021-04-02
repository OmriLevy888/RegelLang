#pragma once
#include "parser/ast/ASTNode.hpp"
#include "common/utils/BitField.hpp"
#include "common/Core.hpp"
#include "common/ILoggable.hpp"
#include <memory>

namespace rgl {
class ConstructNode : public ASTNode {
};

using Construct = std::unique_ptr<ConstructNode>;
}; // namespace rgl