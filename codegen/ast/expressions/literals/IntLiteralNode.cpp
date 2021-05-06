#include "parser/ast/expressions/literals/IntLiteralNode.hpp"
#include "llvm/IR/Constants.h"

namespace rgl {
// llvm::Value *IntLiteralNode::genCode() { return llvm::ConstantInt(); }
llvm::Value *IntLiteralNode::genCode() { return nullptr; }
}; // namespace rgl
