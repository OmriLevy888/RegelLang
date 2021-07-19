#include "parser/ast/expressions/literals/class-literal/ClassLiteralNode.hpp"
#include "codegen/values/ValueBase.hpp"

namespace rgl {
ValuePtr ClassLiteralNode::define() { return ValueBase::BadValue(); }
}; // namespace rgl
