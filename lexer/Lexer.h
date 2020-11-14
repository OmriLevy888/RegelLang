#pragma once
#include "common/Core.h"
#include "common/collections/source-stream/ISourceStream.h"
#include "lexer/ITokenGenerator.h"


namespace rgl {
class Lexer : public ITokenGenerator {
public:
    Lexer(std::unique_ptr<ISourceStream>&& sourceStream)
    : m_sourceStream(std::move(sourceStream))
    {}

    Token getNext() override;

    std::string toString() const override {
        return "Lexer<sourceStream: " + m_sourceStream->toString() + ">";
    }

private:
    std::unique_ptr<ISourceStream> m_sourceStream;
};
}  // namespace rgl