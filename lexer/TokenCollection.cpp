#include "lexer/TokenCollection.hpp"

namespace rgl {
void TokenCollection::saveAnchor() {
  if (m_anchors.size() == 0) {
    m_baseIndex = m_actualIndex;
  }
  m_anchors.push(m_actualIndex);
}

void TokenCollection::saveAnchorAndCurrentToken() {
  saveAnchor();
  m_backlog.emplace_back(m_curr, m_value);
}

void TokenCollection::restoreAnchor() {
  m_backlogIndex = m_anchors.top() - m_baseIndex;
  m_anchors.pop();
  m_usingBacklog = true;
  getNext(); // update m_curr and m_value
}

void TokenCollection::discardAnchor() {
  size_t newEnd = m_anchors.top() - m_baseIndex;
  m_backlog.erase(m_backlog.begin() + newEnd, m_backlog.end());
  m_anchors.pop();
  if (0 == m_anchors.size()) {
    m_usingBacklog = false;
  }
}

size_t TokenCollection::peekAnchor() {
  return m_anchors.size() == 0 ? -1 : m_anchors.top();
}

const Token &TokenCollection::getNext() {
  if (!m_usingBacklog) { // when not using the backlog
    m_actualIndex++;
    auto tvp = m_tokenGenerator->getNext();
    m_curr = tvp.m_token;
    m_value = std::move(tvp.m_value);

    // after getting the next token, save it if there are any anchors
    if (m_anchors.size() != 0) {
      m_backlog.emplace_back(m_curr, m_value);
    }
    return m_curr;
  } else {
    // using the backlog, the the current token from the backlog
    m_curr = m_backlog[m_backlogIndex].m_token;
    m_value = m_backlog[m_backlogIndex++].m_value;
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
