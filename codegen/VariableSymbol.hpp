#pragma once
#include "codegen/SymbolBase.hpp"
#include "parser/ast/constructs/Type.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include <llvm/IR/Value.h>
#include <memory>

namespace rgl {
class VariableSymbol;
using VariableSymbolPtr = std::shared_ptr<VariableSymbol>;

class VariableSymbol : public SymbolBase {
public:
  static VariableSymbolPtr make(const std::vector<std::string> &name,
                                TypePtr type);

  llvm::Value *getStoreLoc() const noexcept { return m_storeLoc; }

  virtual std::string toString() const override;

private:
  TypePtr m_type;
  llvm::Value *m_storeLoc;

  VariableSymbol(TypePtr type, llvm::Value *storeLoc);
};
}; // namespace rgl
