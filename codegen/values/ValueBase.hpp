#pragma once
#include "common/ILoggable.hpp"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <optional>
#include <variant>
namespace rgl {
class ValueBase;
using ValuePtr = std::shared_ptr<ValueBase>;

class ValueBase : public ILoggable {
public:
  static ValuePtr BadValue() { return ValuePtr(new ValueBase(false)); }
  static ValuePtr Success() { return ValuePtr(new ValueBase(true)); }

  operator bool() const noexcept { return success(); }
  bool success() const noexcept { return m_success; }

  llvm::Value *llvmValue() const noexcept {
    return std::get<llvm::Value *>(m_contained.value());
  }
  llvm::Type *llvmType() const noexcept {
    if (holds_alternative<llvm::Value *>()) {
      return llvmValue()->getType();
    } else {
      return std::get<llvm::Type *>(m_contained.value());
    }
  }

  llvm::Module *llvmModule() const noexcept {
    return std::get<llvm::Module *>(m_contained.value());
  }

  virtual bool isCallable() const { return false; }
  virtual bool isFunction() const { return false; }
  virtual bool isMacro() const { return false; }
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
    } else if (holds_alternative<llvm::Value *>()) {
      rso << *llvmValue();
    } else {
      rso << *llvmType();
    }
    rso.flush();
    return str;
  }

protected:
  std::optional<std::variant<llvm::Value *, llvm::Module *, llvm::Type *>>
      m_contained;
  bool m_success;

  explicit ValueBase(bool success) : m_contained(std::nullopt), m_success(success) {}
  ValueBase(llvm::Value *llvmValue)
      : m_contained(llvmValue), m_success(llvmValue != nullptr) {}
  ValueBase(llvm::Module *llvmModule)
      : m_contained(llvmModule), m_success(llvmModule != nullptr) {}
  ValueBase(llvm::Type *llvmType)
      : m_contained(llvmType), m_success(llvmType != nullptr) {}

private:
  template<typename T>
  bool holds_alternative() const {
    if (!m_contained.has_value()) {
      return false;
    }

    return std::holds_alternative<T>(m_contained.value());
  }
};
}; // namespace rgl
