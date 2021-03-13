#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ParserUtilities.hpp"
#include "parser/ast/Type.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
TypePtr Parser::parseType(bool skipQualifiers) {
  BitField<TypeProperties> typeProperties = TypeProperties::_default;
  if (!skipQualifiers) {
    switch (m_tokens->getCurr()) {
    case TokenType::t_ampersand:
      typeProperties = TypeProperties::_mutable;
      m_tokens->getNext(); // consume type qualifier
      break;
    case TokenType::t_colon:
      typeProperties = TypeProperties::_owning;
      m_tokens->getNext(); // consume type qualifier
      break;
    default:
      break;
    }
  } else {
    typeProperties = TypeProperties::_owning;
  }

  switch (m_tokens->getCurr()) {
  case TokenType::t_lesser_than: // <>T unique pointer
    if (TokenType::t_greater_than != m_tokens->getNext()) {
      ErrorManager::logError(
          ErrorTypes::E_BAD_TOKEN,
          {Formatter("Expected '>', found {}", tokenToString(m_tokens)),
           m_tokens});
      return nullptr;
    }
    typeProperties |= TypeProperties::_isPointer;
    m_tokens->getNext(); // consume >
    break;
  case TokenType::t_open_bracket: // {}T shared pointer :TODO: add {TKey =>
                                  // TVal} map
    if (TokenType::t_greater_than != m_tokens->getNext()) {
      ErrorManager::logError(
          ErrorTypes::E_BAD_TOKEN,
          {Formatter("Expected '>', found {}", tokenToString(m_tokens)),
           m_tokens});
      return nullptr;
    }
    typeProperties |= TypeProperties::_isPointer;
    typeProperties |= TypeProperties::_isShared;
    break;
    // TODO: add [T] list
  default:
    break;
  }

  std::vector<std::string> name;
  if (!ParserUtilities::isIdentifier(m_tokens->getCurr())) {
    ErrorManager::logError(
        ErrorTypes::E_BAD_TOKEN,
        {Formatter("Expected identifier, found {}", tokenToString(m_tokens)),
         m_tokens});
    return nullptr;
  }
  name.push_back(
      std::get<std::string>(std::move(m_tokens->getCurrValue().value())));
  m_tokens->getNext(); // consume first identifier

  while (TokenType::t_dot == m_tokens->getCurr()) {
    if (TokenType::t_identifier != m_tokens->getNext()) {
      ErrorManager::logError(
          ErrorTypes::E_BAD_TOKEN,
          {Formatter("Expected identifier, found {}", tokenToString(m_tokens)),
           m_tokens, "Did you add an extra dot ('.')?"});
      return nullptr;
    }

    name.push_back(
        std::get<std::string>(std::move(m_tokens->getCurrValue().value())));
    m_tokens->getNext(); // consume current identifier
  }

  return makeType(std::move(name), typeProperties);
}
}; // namespace rgl
