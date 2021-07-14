#include "common/errors/ErrorManager.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/ast/expressions/VarDeclNode.hpp"
#include <vector>

namespace rgl {
ClassPtr Parser::parseClass() {
  if (TokenType::t_class != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume `class`

  Identifier name = nullptr; // indicates an anonymous class
  if (TokenType::t_identifier == m_tokens->getCurr()) {
    name = parseIdentifier();
  }

  if (TokenType::t_open_bracket != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume {

  std::vector<FieldPtr> fields{};
  std::vector<MethodPtr> methods{};

  while (TokenType::t_close_bracket != m_tokens->getCurr()) {
    bool isExposed = (TokenType::t_pub == m_tokens->getCurr());
    if (isExposed) {
      m_tokens->getNext(); // consume `pub`
    }

    switch (m_tokens->getCurr()) {
    case TokenType::t_func:
      if (!parseMethod(isExposed, methods)) {
        return nullptr;
      }
      break;
    case TokenType::t_hook:
      // TODO: implement
      ErrorManager::logError(ErrorTypes::E_NOT_YET_IMPLEMENTED,
                             "Hooks are not implemented yet");
      return nullptr;
    default:
      if (!parseField(isExposed, fields)) {
        return nullptr;
      }
      break;
    }
  }

  if (TokenType::t_close_bracket != m_tokens->getCurr()) {
    // TODO: write error message
    return nullptr;
  }
  m_tokens->getNext(); // consume }

  return std::make_unique<ClassLiteralNode>(std::move(name), std::move(fields),
                                            std::move(methods));
}

bool Parser::parseField(bool isExposed, std::vector<FieldPtr> &fields) {
  // [pub] [var] [[a[, b[...]]]] : foo;
  bool isMutable = false;
  switch (m_tokens->getCurr()) {
  case TokenType::t_var:
    m_tokens->saveAnchor();
    isMutable = true;
    if (TokenType::t_open_square != m_tokens->getNext()) {
      m_tokens->restoreAnchor();
      break;
    }
  case TokenType::t_open_square:
    return parseFieldMultipleShorthand(isMutable, isExposed, fields);
  default:
    break;
  }

  // [pub] [var] a : foo[, [var] b : bar[...]];
  std::vector<VarDeclPtr> fieldDeclarations;
  while (TokenType::t_semicolon != m_tokens->getCurr()) {
    if (fieldDeclarations.size() != 0) {
      if (TokenType::t_comma != m_tokens->getCurr()) {
        // TODO: write error message
        return false;
      }
      m_tokens->getNext(); // consume `,`
    }

    if (TokenType::t_identifier != m_tokens->getCurr()) {
      // TODO: write error message
      return false;
    }
    auto fieldName = parseIdentifier();

    auto varDecl = parseVarDecl(std::move(fieldName), true, false);
    if (nullptr == varDecl) {
      // TODO: write error message
      return false;
    }
    fieldDeclarations.push_back(std::move(varDecl));
  }
  m_tokens->getNext(); // consume `;`

  for (auto &&varDecl : std::move(fieldDeclarations)) {
    fields.push_back(varDecl->toFieldPtr(isExposed, isMutable));
  }

  return true;
}

bool Parser::parseFieldMultipleShorthand(bool isMutable, bool isExposed,
                                         std::vector<FieldPtr> &fields) {
  std::vector<Identifier> fieldNames{};
  m_tokens->getNext(); // consume `[`

  while (TokenType::t_close_square != m_tokens->getCurr()) {
    if (0 != fieldNames.size()) {
      if (TokenType::t_comma != m_tokens->getCurr()) {
        // TODO: write error message
        return false;
      }
      m_tokens->getNext(); // consume `,`
    }

    if (TokenType::t_identifier != m_tokens->getCurr()) {
      // TODO: write error message
      return false;
    }

    fieldNames.push_back(parseIdentifier());
  }
  m_tokens->getNext(); // consume `]`

  if (TokenType::t_colon != m_tokens->getCurr()) {
    // TODO: write error message
    return false;
  }
  m_tokens->getNext();

  auto type = parseType();
  if (nullptr == type) {
    // TODO: write error message
    return false;
  }

  type = type->getOwningType();
  if (isMutable) {
    type = type->getMutableType();
  }

  if (TokenType::t_semicolon != m_tokens->getCurr()) {
    // TODO: write error mesage
    return false;
  }
  m_tokens->getNext(); // consume `;`

  for (auto &&name : std::move(fieldNames)) {
    fields.push_back(std::make_unique<ClassFieldNode>(
        isExposed, std::move(type), std::move(name)));
  }

  return true;
}

bool Parser::parseMethod(bool isExposed, std::vector<MethodPtr> &methods) {
  auto func = parseFunction();
  if (nullptr == func) {
    return false;
  }

  BitField<MethodProperties> methodProperties = MethodProperties::_default;

  methods.push_back(std::make_unique<MethodNode>(isExposed, std::move(func),
                                                 methodProperties));
  return true;
}
}; // namespace rgl
