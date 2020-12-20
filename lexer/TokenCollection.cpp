#include "lexer/TokenCollection.hpp"

namespace rgl {
void TokenCollection::saveAnchor() {
  if (m_anchors.size() == 0) {
    m_baseIndex = m_actualIndex;
  }
  m_anchors.push(m_actualIndex);
}
void TokenCollection::restoreAnchor() {
  m_backlogIndex = m_anchors.top() - m_baseIndex;
  m_anchors.pop();
  m_usingBacklog = true;
}
size_t TokenCollection::peekAnchor() {
  return m_anchors.size() == 0 ? -1 : m_anchors.top();
}
const Token &TokenCollection::getNext() {
  if (!m_usingBacklog) { // when not using the backlog
    m_actualIndex++;
    auto [m_curr, m_value] = m_tokenGenerator->getNext();

    // after getting the next token, save it if there are any anchors
    if (m_anchors.size() != 0) {
      m_backlog.push_back(m_curr);
    }
    return m_curr;
  } else {
    // using the backlog, the the current token from the backlog
    Token ret = m_backlog[m_backlogIndex++];
    if (m_backlogIndex >=
        m_backlog.size()) { // check if finished current anchor
      m_usingBacklog = false;
      if (m_anchors.size() == 0) { // if finished all anchors, clear backlog
        m_backlog.clear();
      }
    }
    return m_curr;
  }
}
} // namespace rgl
