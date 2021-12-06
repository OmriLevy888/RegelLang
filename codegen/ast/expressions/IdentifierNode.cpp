#include "parser/ast/expressions/IdentifierNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/BasicValue.hpp"
#include "codegen/values/symbols/VariableSymbol.hpp"

namespace rgl {
ValuePtr IdentifierNode::genCode() {
  std::vector<std::string> name = this->get();

  // search stack frames of current function
  auto stackFrames =
      Context::getCurrContext()->getCurrGeneratedFunction()->getStackFrames();
  for (auto frame = stackFrames.crbegin(); frame != stackFrames.crend();
       frame++) {
    auto variable = (*frame)->get(name);
    if (nullptr != variable) {
      auto variableSymbol = std::dynamic_pointer_cast<VariableSymbol>(variable);
      auto storeLoc = variable->llvmValue();
      if (variableSymbol->isParameter()) {
        // paramaters are not pointers, do not load them
        return std::make_shared<BasicValue>(storeLoc);
      }
      return std::make_shared<BasicValue>(Context::builder()->CreateLoad(
          variable->llvmType(), storeLoc,
          Formatter("{}@load", variableSymbol->getName()).toString()));
    }
  }

  // look for globals
  auto global = Context::module()->symbols().get(name);
  if (nullptr != global) {
    if (global->isFunction()) {
      return global;
    } else if (global->isVariable()) { // global variable
      auto globalVarialbe = std::dynamic_pointer_cast<VariableSymbol>(global);
      return std::make_shared<BasicValue>(Context::builder()->CreateLoad(
          globalVarialbe->llvmType(), globalVarialbe->llvmValue(),
          Formatter("{}@load", globalVarialbe->getName()).toString()));
    }

    // TODO: wrtie error message
    return ValueBase::BadValue();
  }

  // TODO: look for types
  return ValueBase::BadValue();
}
}; // namespace rgl
