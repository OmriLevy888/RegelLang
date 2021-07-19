#include "parser/ast/statements/StatementNode.hpp"
#include "codegen/values/ValueBase.hpp"

namespace rgl {
ValuePtr StatementNode::genCode() { return ValueBase::BadValue(); }
}; // namespace rgl
