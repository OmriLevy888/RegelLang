#include "parser/ast/expressions/ScopeNode.hpp"
#include "codegen/Context.hpp"
#include "codegen/values/ValueBase.hpp"
#include "codegen/values/symbols/SymbolMap.hpp"
#include <iostream>

namespace rgl {
template <typename TDeclarable>
ValuePtr defineDeclareLoop(const std::vector<TDeclarable> &declarables) {
  for (auto &node : declarables) {
    node->declare();
  }

  for (auto &node : declarables) {
    std::cout << node->toString() << std::endl;
    auto ret = node->define();
    if (!ret->success()) {
      return ret;
    }
  }

  return ValueBase::Success();
}

ValuePtr ScopeNode::genCode() {
  auto currFunction = Context::getCurrContext()->getCurrGeneratedFunction();

  if (nullptr != currFunction) {
    // when generating code in some function
    currFunction->createStackFrame();
  }

  auto ret = defineDeclareLoop(m_classes);
  if (!ret->success()) {
    return ret;
  }
  ret = defineDeclareLoop(m_functions);
  if (!ret->success()) {
    return ret;
  }

  for (auto &statement : m_statements) {
    ret = statement->genCode();
    if (!ret->success()) {
      return ret;
    }
  }

  if (nullptr != currFunction) {
    currFunction->removeStackFrame();
  }

  return ValueBase::Success();
}
}; // namespace rgl
