#pragma once
#include "common/ILoggable.hpp"
#include "common/source-objects/SourceLocation.hpp"
#include "common/source-objects/SourceRange.hpp"

#include <vector>

namespace rgl {
class TokenValuePair;

class SourceLine : public ILoggable {
public:
  SourceLine(const std::string &repr);
  SourceLine(std::string &&repr);
  SourceLine(std::string &&repr, const std::vector<TokenValuePair> &tokens);
  SourceLine(std::string &&repr, const std::vector<TokenValuePair> &tokens,
             const size_t lineNo);

  std::string::const_iterator cbegin() const { return m_repr.cbegin(); }
  std::string::iterator begin() { return m_repr.begin(); }
  std::string repr() const { return m_repr; }
  
  
  std::vector<SourceLocation> &locations() { return m_locations; }

  std::string pointAt(const SourceRange &range) const;

  virtual std::string toString() const override;

private:
  std::string m_repr;
  std::vector<SourceLocation> m_locations;
};
} // namespace rgl
