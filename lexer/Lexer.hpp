#pragma once
#include "common/Core.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "common/collections/source-stream/ISourceStream.hpp"
#include "lexer/ITokenGenerator.hpp"

namespace rgl {
enum class LexerState : uint8_t { normal, multilineComment };

class Lexer : public ITokenGenerator {
public:
  Lexer(std::unique_ptr<ISourceStream> &&sourceStream,
        std::shared_ptr<SourceProject> project)
      : m_sourceStream(std::move(sourceStream)), m_project(project),
        m_state(LexerState::normal), m_yieldedEof(false),
        m_value(std::nullopt) {
    m_file = m_project->m_files.begin() + m_sourceStream->getFileIndex();
  }

  virtual TokenValuePair getNext() override;
  virtual std::shared_ptr<SourceProject> getSourceProject() const override {
    return m_project;
  }

  virtual std::string toString() const override {
    return "Lexer<sourceStream: " + m_sourceStream->toString() + ">";
  }

private:
  std::unique_ptr<ISourceStream> m_sourceStream;
  std::shared_ptr<SourceProject> m_project;
  std::vector<SourceFile>::iterator m_file;
  std::vector<SourceLine>::iterator m_currLine;
  size_t m_currTokenIdx;
  size_t m_currLineIdx;
  size_t m_pos;
  LexerState m_state;
  std::optional<TokenValue> m_value;

  Token m_eof;
  bool m_yieldedEof;

  Token makeToken(TokenType type, uint32_t reprStartIdx = 0,
                  uint16_t reprLen = 0) const;
  Token getNextImpl();

  bool skipWhiteSpace();
  void addToken(const Token &token);

  bool lexComment();

  bool lexKeyword(Token &ret);
  bool lexIdentifier(Token &ret);
  bool lexSpecialCharacter(Token &ret);
  bool lexOperator(Token &ret);
  bool lexLiteral(Token &ret);

  bool lexCharacter(char &value);
  bool lexCharLiteral(Token &ret);
  bool lexIntLiteral(Token &ret);
  bool lexRealLiteral(Token &ret);
  bool lexStringLiteral(Token &ret);

  bool isHex(const char value) const noexcept;
  uint8_t digitToValue(const char digit) const noexcept;
};
}; // namespace rgl
