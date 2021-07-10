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
                                TypePtr type,
                                llvm::Value *paramValue = nullptr);

  virtual bool isVariable() const override { return true; }
  virtual bool isParameter() const override { return m_isParameter; }

  llvm::Value *getStoreLoc() const noexcept { return m_storeLoc; }
  std::string getName() const { return m_name; }

  virtual std::string toString() const override;

private:
  std::string m_name;
  TypePtr m_type;
  llvm::Value *m_storeLoc;
  bool m_isParameter;

  VariableSymbol(const std::string &name, TypePtr type, llvm::Value *storeLoc,
                 bool isParameter);
};
}; // namespace rgl
