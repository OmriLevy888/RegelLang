#pragma once
#include "common/ILoggable.hpp"
#include "parser/ast/constructs/FunctionType.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"
#include "llvm/IR/Function.h"
#include <optional>
#include <unordered_map>

namespace rgl {
class FunctionTableEntry;
using FunctionTableEntryPtr = std::shared_ptr<FunctionTableEntry>;

class FunctionTableMap : public ILoggable {
public:
  FunctionTableMap();

  FunctionTableEntryPtr get(const std::vector<std::string> &name);
  void insert(const std::vector<std::string> &name,
              FunctionTableEntryPtr function);

  virtual std::string toString() const override;

private:
  std::unordered_map<std::string, std::shared_ptr<FunctionTableMap>> m_maps;
  FunctionTableEntryPtr m_function;

  FunctionTableEntryPtr get(std::vector<std::string>::const_iterator curr,
                            std::vector<std::string>::const_iterator end);
  void insert(std::vector<std::string>::const_iterator curr,
              std::vector<std::string>::const_iterator end,
              FunctionTableEntryPtr function);
};

class FunctionTableEntry : public ILoggable {
public:
  static FunctionTableEntryPtr make(const std::vector<std::string> &name,
                                    TypePtr retType,
                                    const std::vector<Parameter> &parameters,
                                    bool isVarArg = false);

  llvm::Function *llvmFunction() { return m_llvmFunction; }

  virtual std::string toString() const override;

private:
  FunctionTypePtr m_type;
  llvm::Function *m_llvmFunction;

  FunctionTableEntry(FunctionTypePtr type, llvm::Function *llvmFunction);
};
}; // namespace rgl
