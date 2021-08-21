#include "common/errors/ErrorObject.hpp"

namespace rgl {
std::string ErrorObject::toString() const {
  std::string formatted = m_message;
  
  if (0 != m_hint.size()) {
    formatted += Formatter("\n    (HINT: {})", m_hint);
  }

  if (TokenType::t_err != m_tok) {
    formatted += Formatter("\n{}", pointAt(m_tok));
  }
  
  return formatted;
}
} // namespace rgl
