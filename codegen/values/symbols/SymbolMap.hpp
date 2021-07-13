#pragma once
#include "codegen/values/symbols/SymbolBase.hpp"
#include "common/ILoggable.hpp"
#include "parser/ast/constructs/TypeNodeBase.hpp"
#include "parser/ast/expressions/ExpressionNode.hpp"
#include "parser/ast/expressions/literals/ParameterNode.hpp"
#include <unordered_map>
#include <vector>

namespace rgl {
class SymbolMap;
using SymbolMapPtr = std::shared_ptr<SymbolMap>;

class FunctionSymbol;
using FunctionSymbolPtr = std::shared_ptr<FunctionSymbol>;

class VariableSymbol;
using VariableSymbolPtr = std::shared_ptr<VariableSymbol>;

class TypeSymbolBase;
using TypeSymbolPtr = std::shared_ptr<TypeSymbolBase>;

class BasicTypeSymbol;
using BasicTypeSymbolPtr = std::shared_ptr<BasicTypeSymbol>;

class FunctionTypeSymbol;
using FunctionTypeSymbolPtr = std::shared_ptr<FunctionTypeSymbol>;

class SymbolMap : public ILoggable {
public:
  SymbolMap();

  SymbolPtr get(const std::vector<std::string> &name);
  SymbolPtr insert(const std::vector<std::string> &name, SymbolPtr symbol);

  TypeSymbolPtr getType(const TypeNodePtr &typeNode);
  FunctionTypeSymbolPtr getFunctionType(std::vector<TypeSymbolPtr> &&params,
                                        TypeSymbolPtr retType);

  FunctionSymbolPtr createFunction(const std::vector<std::string> &name,
                                   TypeSymbolPtr retType,
                                   const std::vector<Parameter> &parameters,
                                   bool isVarArg = false);
  VariableSymbolPtr createVariable(const std::vector<std::string> &name,
                                   const Expression &value,
                                   TypeSymbolPtr type = nullptr);
  VariableSymbolPtr createParameter(const std::vector<std::string> &name,
                                    TypeSymbolPtr type,
                                    llvm::Value *paramValue);

  void clean();

  virtual std::string toString() const override;

private:
  std::unordered_map<std::string, SymbolMapPtr> m_maps;
  // TODO: replace with stack(?)
  SymbolPtr m_symbol;

  SymbolPtr get(std::vector<std::string>::const_iterator curr,
                std::vector<std::string>::const_iterator end);
  SymbolPtr insert(std::vector<std::string>::const_iterator curr,
                   std::vector<std::string>::const_iterator end,
                   SymbolPtr symbol);
};
}; // namespace rgl
