#pragma once
#include "common/ILoggable.hpp"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <variant>

namespace rgl {
class ValueBase;
using ValuePtr = std::shared_ptr<ValueBase>;

class ValueBase : public ILoggable {
public:
  static ValuePtr BadValue() { return ValuePtr(new ValueBase()); }

  operator bool() const noexcept { return success(); }
  bool success() const noexcept { return m_success; }

  llvm::Value *llvmValue() const noexcept {
    return std::get<llvm::Value *>(m_contained);
  }
  llvm::Type *llvmType() const noexcept {
    if (std::holds_alternative<llvm::Value *>(m_contained)) {
      return llvmValue()->getType();
    } else {
      return std::get<llvm::Type *>(m_contained);
    }
  }

  llvm::Module *llvmModule() const noexcept {
    return std::get<llvm::Module *>(m_contained);
  }

  virtual bool isFunction() const { return false; }
  virtual bool isNamespace() const { return false; }
  virtual bool isVariable() const { return false; }
  virtual bool isParameter() const { return false; }
  virtual bool isModule() const { return false; }
  virtual bool isType() const { return false; }

  virtual std::string toString() const override {
    std::string str;
    llvm::raw_string_ostream rso{str};
    if (isModule()) {
      rso << *llvmModule();
    } else if (std::holds_alternative<llvm::Value *>(m_contained)) {
      rso << *llvmValue();
    } else {
      rso << *llvmType();
    }
    rso.flush();
    return str;
  }

protected:
  std::variant<std::nullptr_t, llvm::Value *, llvm::Module *, llvm::Type *>
      m_contained;
  bool m_success;

  ValueBase() : m_contained(nullptr), m_success(false) {}
  ValueBase(llvm::Value *llvmValue, bool success)
      : m_contained(llvmValue), m_success(success) {}
  ValueBase(llvm::Module *llvmModule, bool success)
      : m_contained(llvmModule), m_success(success) {}
  ValueBase(llvm::Type *llvmType, bool success)
      : m_contained(llvmType), m_success(success) {}
};
}; // namespace rgl
