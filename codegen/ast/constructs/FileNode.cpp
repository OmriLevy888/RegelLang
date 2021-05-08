#include "parser/ast/constructs/FileNode.hpp"
#include "codegen/Context.hpp"
#include <iostream>

namespace rgl {
void FileNode::genCode() {
  // TODO: implement top level expressions and classes
  if (nullptr != m_namespace) {
    std::cout << "haha" << std::endl;
    Context::setCurrContext(m_namespace->getName());
  }
  std::cout << "mhm" << std::endl;

  for (auto &func : m_functions) {
    func->genCode();
  }
}
}; // namespace rgl
