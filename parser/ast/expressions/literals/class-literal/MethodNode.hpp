#include "common/utils/BitField.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/literals/FunctionLiteralNode.hpp"

namespace rgl {
enum class MethodProperties : uint32_t { _virtual };

class MethodNode : public ExpressionNode {
public:
  MethodNode(bool isExposed, FunctionPtr func,
             BitField<MethodProperties> properties)
      : m_isExposed(isExposed), m_func(std::move(func)),
        m_properties(properties) {}

  virtual std::string toTreeStr(size_t spaces) const override;

private:
  bool m_isExposed;
  BitField<MethodProperties> m_properties;
  FunctionPtr m_func;
};

using MethodPtr = std::unique_ptr<MethodNode>;
}; // namespace rgl

namespace std {
std::string to_string(rgl::BitField<rgl::MethodProperties> properties);
}; // namespace std
