#include "parser/ast/constructs/FileNode.hpp"

namespace rgl {
std::string FileNode::toTreeStr(size_t spaces) const {
  std::string spacesStr(spaces + 9, ' ');
  std::string namespaceStr = (nullptr == m_namespace)
                                 ? ("global-namespace")
                                 : (m_namespace->toTreeStr(spaces));
  std::string classesStr{};
  if (0 == m_classes.size()) {
    classesStr = "no-classes";
  } else {
    auto iter = m_classes.cbegin();
    for (; iter + 1 != m_classes.cend(); iter++) {
      classesStr += Formatter("{},\n{}", (*iter)->toTreeStr(spaces + 17),
                              std::string(spaces + 17, ' '));
    }
    classesStr += (*iter)->toTreeStr(spaces + 17);
  }

  std::string functionsStr{};
  if (0 == m_functions.size()) {
    functionsStr = "no-functions";
  } else {
    auto iter = m_functions.cbegin();
    for (; iter + 1 != m_functions.cend(); iter++) {
      functionsStr += Formatter("{},\n{}", (*iter)->toTreeStr(spaces + 19),
                                std::string(spaces + 19, ' '));
    }
    functionsStr += (*iter)->toTreeStr(spaces + 19);
  }

  std::string globalsStr{};
  if (0 == m_topLevelExpressions.size()) {
    globalsStr = "no-globals";
  } else {
    auto iter = m_topLevelExpressions.cbegin();
    for (; iter + 1 != m_topLevelExpressions.cend(); iter++) {
      globalsStr += Formatter("{},\n{}", (*iter)->toTreeStr(spaces + 17),
                              std::string(spaces + 17, ' '));
    }
    globalsStr += (*iter)->toTreeStr(spaces + 17);
  }

  return Formatter(
      "FileNode<namespace:{},\n{}classes:{},\n{}functions:{},\n{}globals:{}>",
      namespaceStr, spacesStr, classesStr, spacesStr, functionsStr, spacesStr,
      globalsStr);
}
}; // namespace rgl
