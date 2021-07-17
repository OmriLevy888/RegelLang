#include "codegen/values/symbols/types/FunctionTypeSymbol.hpp"
#include "codegen/Context.hpp"
#include "parser/ast/constructs/FunctionTypeNode.hpp"
#include "llvm/IR/Type.h"

namespace rgl {
FunctionTypeSymbol::FunctionTypeSymbol(llvm::Type *llvmType,
                                       BitField<TypeTraits> traits,
                                       TypeSymbolPtr retType,
                                       std::vector<TypeSymbolPtr> &&paramTypes)
    : TypeSymbolBase(llvmType, traits), m_retType(retType),
      m_paramTypes(std::move(paramTypes)) {}

FunctionTypeSymbolPtr FunctionTypeSymbol::make(const TypeNodePtr &typeNode) {
  if (!typeNode->isFunctionType()) {
    // TODO: write error message
    return nullptr;
  }
  // NOTE: using raw pointer to make sure ownership is not taken
  auto functionTypeNode = dynamic_cast<FunctionTypeNode *>(typeNode.get());

  auto retType =
      Context::module()->symbols().getType(functionTypeNode->retType());
  std::vector<TypeSymbolPtr> paramTypes;
  std::vector<llvm::Type *> llvmParamTypes;
  const size_t paramsCount = functionTypeNode->paramTypes().size();
  paramTypes.reserve(paramsCount);
  llvmParamTypes.reserve(paramsCount);
  for (const auto &type : functionTypeNode->paramTypes()) {
    auto currTypeSymbol = Context::module()->symbols().getType(type);
    paramTypes.push_back(currTypeSymbol);
    llvmParamTypes.push_back(currTypeSymbol->llvmType());
  }

  const bool isVarArg = false;
  auto llvmFunctionType = llvm::FunctionType::get(
      retType->llvmType(), std::move(llvmParamTypes), isVarArg);

  const BitField<TypeTraits> traits = TypeTraits::TriviallyCopiable;
  return FunctionTypeSymbolPtr(new FunctionTypeSymbol(
      llvmFunctionType, traits, retType, std::move(paramTypes)));
}
FunctionTypeSymbolPtr
FunctionTypeSymbol::make(TypeSymbolPtr retType,
                         std::vector<TypeSymbolPtr> &&paramTypes) {
  const bool isVarArg = false;
  std::vector<llvm::Type *> llvmParamTypes;
  llvmParamTypes.reserve(paramTypes.size());
  for (const auto &type : paramTypes) {
    llvmParamTypes.push_back(type->llvmType());
  }
  auto llvmFunctionType = llvm::FunctionType::get(
      retType->llvmType(), std::move(llvmParamTypes), isVarArg);

  const BitField<TypeTraits> traits = TypeTraits::TriviallyCopiable;
  return FunctionTypeSymbolPtr(new FunctionTypeSymbol(
      llvmFunctionType, traits, retType, std::move(paramTypes)));
}

bool FunctionTypeSymbol::operator==(TypeSymbolPtr other) const {
  if (other.get() == this) {
    return true;
  }

  if (!other->isFunctionType()) {
    return false;
  }
  auto otherFunctionTypeSymbol =
      std::dynamic_pointer_cast<FunctionTypeSymbol>(other);

  if (otherFunctionTypeSymbol->retType() != this->retType()) {
    return false;
  }

  auto thisParamIt = this->m_paramTypes.cbegin();
  auto otherParamIt = otherFunctionTypeSymbol->m_paramTypes.cbegin();
  for (; thisParamIt != this->m_paramTypes.cend() &&
         otherParamIt != otherFunctionTypeSymbol->m_paramTypes.cend();
       thisParamIt++, otherParamIt++) {
    if (!(*thisParamIt)->operator==(*otherParamIt)) {
      return false;
    }
  }

  return true;
}
}; // namespace rgl
