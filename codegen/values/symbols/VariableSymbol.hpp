#pragma once
#include "codegen/values/symbols/SymbolBase.hpp"
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
                                TypePtr type,
                                llvm::Value *paramValue = nullptr);

  virtual bool isVariable() const override { return true; }
  virtual bool isParameter() const override { return m_isParameter; }

  std::string getName() const { return m_name; }

  virtual std::string toString() const override;

private:
  std::string m_name;
  bool m_isParameter;

  VariableSymbol(const std::string &name, llvm::Value *storeLoc,
                 bool isParameter);
};
}; // namespace rgl
