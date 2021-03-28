#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ParserUtilities.hpp"
#include "parser/ast/constructs/FunctionType.hpp"
#include "parser/ast/constructs/Type.hpp"
#include "parser/ast/expressions/IdentifierNode.hpp"

namespace rgl {
TypePtr Parser::parseType(bool skipQualifiers) {
  std::optional<BitField<TypeProperties>> properties = TypeProperties::_default;
  if (!skipQualifiers) {
    properties = parseTypeModifiers();
    if (!properties.has_value()) {
      // TODO: add error message
      return nullptr;
    }
  } else {
    properties = TypeProperties::_owning;
  }

  switch (m_tokens->getCurr()) {
    case TokenType::t_func:
      m_tokens->getNext();  // consume func keyword
      return parseFunctionType(properties.value());
    default:
      return parseBasicType(properties.value());
  }
}

std::optional<BitField<TypeProperties>> Parser::parseTypeModifiers() {
  BitField<TypeProperties> properties = TypeProperties::_default;
  switch (m_tokens->getCurr()) {
    case TokenType::t_ampersand:
      properties = TypeProperties::_mutable;
      m_tokens->getNext();  // consume type qualifier
      break;
    case TokenType::t_colon:
      properties = TypeProperties::_owning;
      m_tokens->getNext();  // consume type qualifier
      break;
    default:
      break;
  }

  switch (m_tokens->getCurr()) {
    case TokenType::t_lesser_than:  // <>T unique pointer
      if (TokenType::t_greater_than != m_tokens->getNext()) {
        ErrorManager::logError(
            ErrorTypes::E_BAD_TOKEN,
            {Formatter("Expected '>', found {}", tokenToString(m_tokens)),
             m_tokens});
        return std::nullopt;
      }
      properties |= TypeProperties::_isPointer;
      m_tokens->getNext();  // consume >
      break;
    case TokenType::t_open_bracket:
      if (TokenType::t_close_bracket != m_tokens->getNext()) {
        ErrorManager::logError(
            ErrorTypes::E_BAD_TOKEN,
            {Formatter("Expected '}', found {}", tokenToString(m_tokens)),
             m_tokens});
        return std::nullopt;
      }
      properties |= TypeProperties::_isPointer;
      properties |= TypeProperties::_isShared;
      m_tokens->getNext();  // consume }
      break;
    default:
      break;
  }

  return properties;
}

TypePtr Parser::parseBasicType(BitField<TypeProperties> properties) {
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
  m_tokens->getNext();  // consume first identifier

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
    m_tokens->getNext();  // consume current identifier
  }

  return BasicType::make(std::move(name), properties);
}

TypePtr Parser::parseFunctionType(BitField<TypeProperties> properties) {
  bool multipleParamTypes = (TokenType::t_open_paren == m_tokens->getCurr());
  std::vector<TypePtr> paramTypes;
  if (multipleParamTypes) {
    m_tokens->getNext();  // consume (
    while (TokenType::t_close_paren != m_tokens->getCurr()) {
      if (0 != paramTypes.size()) {  // if not the first parameter, make sure
        // the is a comma
        if (TokenType::t_comma != m_tokens->getCurr()) {
          ErrorManager::logError(
              ErrorTypes::E_BAD_TOKEN,
              {Formatter("Expected ',', found {}", tokenToString(m_tokens)),
               m_tokens, "Did you forget a comma (',')?"});
          return nullptr;
        }
        m_tokens->getNext();  // consume ,
      }

      auto currType = parseType();
      if (nullptr == currType) {
        return nullptr;
      }
      paramTypes.push_back(currType);
    }

    if (TokenType::t_close_paren != m_tokens->getCurr()) {
      ErrorManager::logError(
          ErrorTypes::E_BAD_TOKEN,
          {Formatter("Expected ), found {}", tokenToString(m_tokens)),
           m_tokens});
      return nullptr;
    }
    m_tokens->getNext();  // consume )
  } else if (TokenType::t_arrow !=
             m_tokens->getCurr()) {  // function types may have zero parameters
                                     // and no parentheses only if there is an
                                     // explicit return type
    auto currType = parseType();
    if (nullptr == currType) {
      return nullptr;
    }
    paramTypes.push_back(currType);
  }

  TypePtr retType = BasicType::t_void();
  if (TokenType::t_arrow == m_tokens->getCurr()) {
    m_tokens->getNext();  // consume =>
    retType = parseType();
    if (nullptr == retType) {
      return nullptr;
    }
  }

  return FunctionType::make(std::move(paramTypes), retType, properties);
}  // namespace rgl
};  // namespace rgl
