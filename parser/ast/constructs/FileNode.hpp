#pragma once
#include "parser/ast/ASTNode.hpp"

namespace rgl {
class FileNode : public ASTNode {
public:
  std::string m_path;

  std::string toString() const { return m_path; }
};
}; // namespace rgl
