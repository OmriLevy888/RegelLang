#include "parser/ast/constructs/FileNode.hpp"
#include "codegen/Context.hpp"
#include <iostream>

namespace rgl {
void FileNode::genCode() {
  // TODO: implement top level expressions and classes
  if (nullptr == m_namespace) {
    Context::setCurrContext();
  } else {
    Context::setCurrContext(m_namespace->getName());
  }

  for (auto &func : m_functions) {
    func->genCode();
  }
}
}; // namespace rgl
