#include "codegen/FunctionTableEntry.hpp"
#include "codegen/Context.hpp"
#include "common/Formatter.hpp"
#include "parser/ast/constructs/FunctionType.hpp"
#include "llvm/Support/raw_ostream.h"

namespace rgl {
FunctionTableMap::FunctionTableMap() : m_maps({}), m_function(nullptr) {}

FunctionTableEntryPtr
FunctionTableMap::get(const std::vector<std::string> &name) {
  return get(name.cbegin(), name.cend());
}
FunctionTableEntryPtr
FunctionTableMap::get(std::vector<std::string>::const_iterator curr,
                      std::vector<std::string>::const_iterator end) {
  if (curr == end) {
    return m_function;
  }

  auto nextMap = m_maps[*curr];
  if (nullptr == nextMap) {
    return nullptr;
  }
  return nextMap->get(curr + 1, end);
}

void FunctionTableMap::insert(const std::vector<std::string> &name,
                              FunctionTableEntryPtr function) {
  insert(name.cbegin(), name.cend(), function);
}
void FunctionTableMap::insert(std::vector<std::string>::const_iterator curr,
                              std::vector<std::string>::const_iterator end,
                              FunctionTableEntryPtr function) {
  if (curr == end) {
    m_function = function;
    return;
  }

  auto nextMap = m_maps[*curr];
  if (nullptr == nextMap) {
    nextMap = std::make_shared<FunctionTableMap>();
    m_maps[*curr] = nextMap;
  }
  return nextMap->insert(curr + 1, end, function);
}

std::string FunctionTableMap::toString() const { return "FunctionTableMap"; }

FunctionTableEntry::FunctionTableEntry(FunctionTypePtr type,
                                       llvm::Function *llvmFunction)
    : m_type(type), m_llvmFunction(llvmFunction) {}

FunctionTableEntryPtr
FunctionTableEntry::make(const std::vector<std::string> &name, TypePtr retType,
                         const std::vector<Parameter> &parameters,
                         bool isVarArg) {
  std::vector<TypePtr> paramTypes{};
  paramTypes.reserve(parameters.size());
  for (const auto &param : parameters) {
    paramTypes.push_back(param->getType());
  }
  auto functionType = FunctionType::make(std::move(paramTypes), retType);
  auto llvmFunctionType = functionType->toLLVMType();

  const std::string functionName = Formatter(
      "{}::{}", Formatter<>::joinContainer('.', Context::module()->getName()),
      Formatter<>::joinContainer('.', name));
  llvm::Function *llvmFunction =
      llvm::Function::Create(llvmFunctionType, llvm::Function::ExternalLinkage,
                             functionName, Context::llvmModule());

  std::vector<std::string> paramNames{};
  paramNames.reserve(parameters.size());
  for (const auto &param : parameters) {
    paramNames.push_back(Formatter<>::joinContainer('.', param->getName()));
  }
  size_t idx = 0;
  for (auto &arg : llvmFunction->args()) {
    arg.setName(paramNames[idx++]);
  }

  return std::shared_ptr<FunctionTableEntry>(
      new FunctionTableEntry(functionType, llvmFunction));
}

std::string FunctionTableEntry::toString() const {
  std::string str;
  llvm::raw_string_ostream rso{str};
  rso << *m_llvmFunction;
  rso.flush();
  return Formatter("FunctionTableEntry<{}>", std::move(str));
}
}; // namespace rgl
