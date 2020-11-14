#pragma once

#include "common/Core.h"
#include "common/collections/source-objects/SourceLine.h"

namespace rgl {
class SourceFile : public ILoggable {
public:
    std::string m_path;
    std::vector<SourceLine> m_lines;

    SourceFile(const std::string &path)
    : m_path(path)
    {}

    std::string toString() const override {
        return Formatter("SourceFile<path: \"{}\", lines: {}>",
            m_path, m_lines.size()).toString();
    }
};
}  // namespace rgl