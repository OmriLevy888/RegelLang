#include "parser/ast/expressions/ScopeNode.hpp"

namespace rgl {
std::string ScopeNode::toTreeStr(size_t spaces) const {
  std::string classesStr = "";
  if (0 != m_classes.size()) {
    std::string spacesStr(spaces + 14, ' ');
    auto it = m_classes.cbegin();
    for (; it + 1 != m_classes.cend(); it++) {
      classesStr +=
          Formatter("{},\n{}", (*it)->toTreeStr(spaces + 14), spacesStr);
    }
    classesStr += (*it)->toTreeStr(spaces + 14);
  } else {
    classesStr = "none";
  }

  std::string functionsStr = "";
  if (0 != m_functions.size()) {
    std::string spacesStr(spaces + 16, ' ');
    auto it = m_functions.cbegin();
    for (; it + 1 != m_functions.cend(); it++) {
      functionsStr +=
          Formatter("{},\n{}", (*it)->toTreeStr(spaces + 16), spacesStr);
    }
    functionsStr += (*it)->toTreeStr(spaces + 16);
  } else {
    functionsStr = "none";
  }

  std::string statementsStr = "";
  if (0 != m_statements.size()) {
    std::string spacesStr(spaces + 18, ' ');
    auto it = m_statements.cbegin();
    for (; it + 1 != m_statements.cend(); it++) {
      statementsStr +=
          Formatter("{},\n{}", (*it)->toTreeStr(spaces + 18), spacesStr);
    }
    statementsStr += (*it)->toTreeStr(spaces + 18);
  } else {
    statementsStr = "none";
  }

  const std::string spacesStr(spaces + 6, ' ');

  return Formatter("Scope<classes:{},\n{}functions:{},\n{}statements:{}>",
                   classesStr, spacesStr, functionsStr, spacesStr,
                   statementsStr);
}
}; // namespace rgl
