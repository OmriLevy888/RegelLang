#include "parser/ast/expressions/IdentifierNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/VariableSymbol.hpp"

namespace rgl {
llvm::Value *IdentifierNode::genCode() {
  std::vector<std::string> name = this->get();

  // Search stack frames of current function
  auto stackFrames =
      Context::getCurrContext()->getCurrGeneratedFunction()->getStackFrames();
  for (auto frame = stackFrames.crbegin(); frame != stackFrames.crend();
       frame++) {
    auto variable = (*frame)->get(name);
    if (nullptr != variable) {
      auto variableSymbol = std::dynamic_pointer_cast<VariableSymbol>(variable);
      auto storeLoc = variableSymbol->getStoreLoc();
      return Context::builder()->CreateLoad(
          storeLoc, Formatter("{}@load", variableSymbol->getName()).toString());
    }
  }
  // TODO: look for globals / types
  return nullptr;
}
}; // namespace rgl
