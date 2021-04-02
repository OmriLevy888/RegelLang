#pragma once
#include "parser/ast/constructs/ConstructNode.hpp"

namespace rgl {
class FileNode : public ConstructNode {
 public:
  std::string m_path;

  // TODO: this will someday contain all the things inside a file and then need
  // to create toTreeStr
  std::string toString() const { return m_path; }
};

using File = std::unique_ptr<FileNode>;
};  // namespace rgl
