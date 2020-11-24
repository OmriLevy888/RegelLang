#pragma once
#include "common/Core.hpp"
#include "common/collections/source-objects/SourceProject.hpp"
#include "common/collections/source-stream/ISourceStream.hpp"
#include "lexer/ITokenGenerator.hpp"

namespace rgl {
class Lexer : public ITokenGenerator {
public:
  Lexer(std::unique_ptr<ISourceStream> &&sourceStream)
      : m_sourceStream(std::move(sourceStream)) {}

  Token getNext() override;

  std::string toString() const override {
    return "Lexer<sourceStream: " + m_sourceStream->toString() + ">";
  }

private:
  std::unique_ptr<ISourceStream> m_sourceStream;
  std::shared_ptr<SourceProject> m_project;
  std::vector<SourceLine>::iterator m_currLine;
  size_t m_currLineIdx;
  size_t m_pos;

  bool skipWhiteSpace();
  void addLine(std::string &&line);
  void addToken(const Token &token);
  uint64_t genTokenId() const;
};
} // namespace rgl
