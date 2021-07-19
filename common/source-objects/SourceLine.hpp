#pragma once
#include "common/ILoggable.hpp"

#include <vector>

namespace rgl {
class Token;
class TokenValuePair;

class SourceLine : public ILoggable {
public:
  SourceLine(const std::string &repr);
  SourceLine(std::string &&repr);
  SourceLine(std::string &&repr, const std::vector<TokenValuePair> &tokens);
  SourceLine(std::string &&repr, const std::vector<TokenValuePair> &tokens,
             const size_t lineNo);

  std::string &repr() { return m_repr; }
  std::string repr() const { return m_repr; }
  std::vector<Token> &tokens() { return m_tokens; }

  std::pair<std::string, std::string> pointAt(const Token &tok) const;

  virtual std::string toString() const override;

private:
  std::string m_repr;
  std::vector<Token> m_tokens;
};
} // namespace rgl
