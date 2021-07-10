#include "parser/ast/expressions/IdentifierNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/VariableSymbol.hpp"

namespace rgl {
llvm::Value *IdentifierNode::genCode() {
  std::vector<std::string> name = this->get();

  // search stack frames of current function
  auto stackFrames =
      Context::getCurrContext()->getCurrGeneratedFunction()->getStackFrames();
  for (auto frame = stackFrames.crbegin(); frame != stackFrames.crend();
       frame++) {
    auto variable = (*frame)->get(name);
    if (nullptr != variable) {
      auto variableSymbol = std::dynamic_pointer_cast<VariableSymbol>(variable);
      auto storeLoc = variableSymbol->getStoreLoc();
      if (variableSymbol->isParameter()) {
        // paramaters are not pointers, do not load them
        return storeLoc;
      }
      return Context::builder()->CreateLoad(
          storeLoc, Formatter("{}@load", variableSymbol->getName()).toString());
    }
  }

  // look for globals
  auto global = Context::module()->symbols().get(name);
  if (nullptr != global) {
    if (global->isFunction()) {
      return std::dynamic_pointer_cast<FunctionSymbol>(global)->llvmFunction();
    } else if (global->isVariable()) { // global variable
      auto globalVarialbe = std::dynamic_pointer_cast<VariableSymbol>(global);
      return Context::builder()->CreateLoad(
          globalVarialbe->getStoreLoc(),
          Formatter("{}@load", globalVarialbe->getName()).toString());
    }

    // TODO: wrtie error message
    return nullptr;
  }

  // TODO: look for types
  return nullptr;
}
}; // namespace rgl
