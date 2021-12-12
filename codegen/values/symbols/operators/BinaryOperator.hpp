#pragma once
#include "codegen/values/symbols/SymbolMap.hpp"
#include "common/ILoggable.hpp"
#include <memory>
#include <unordered_map>

namespace rgl {
class BinaryOperator : public ILoggable {
public:
  virtual std::string toString() const override;

private:
  std::unordered_map<TypeSymbolPtr, BinaryOperatorPtr> m_ops;
  CallableSymbolPtr m_symbol;
};
}; // namespace rgl
