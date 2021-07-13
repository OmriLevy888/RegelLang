#pragma once
#include "codegen/values/symbols/SymbolBase.hpp"
#include "codegen/values/symbols/types/TypeSymbolBase.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include <llvm/IR/Value.h>
#include <memory>

namespace rgl {
class VariableSymbol;
using VariableSymbolPtr = std::shared_ptr<VariableSymbol>;

class VariableSymbol : public SymbolBase {
public:
  static VariableSymbolPtr make(const std::vector<std::string> &name,
                                TypeSymbolPtr type,
                                llvm::Value *paramValue = nullptr);

  virtual bool isVariable() const override { return true; }
  virtual bool isParameter() const override { return m_isParameter; }

  std::string getName() const { return m_name; }
  TypeSymbolPtr getType() const { return m_type; }

  virtual std::string toString() const override;

private:
  std::string m_name;
  TypeSymbolPtr m_type;
  bool m_isParameter;

  VariableSymbol(llvm::Value *storeLo, const std::string &name,
                 TypeSymbolPtr type, bool isParameter);
};
}; // namespace rgl
