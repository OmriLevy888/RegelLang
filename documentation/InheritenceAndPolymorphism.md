## Inheritence & Polymorphism
________________________________________________________________________________

To declare a virtual function, use the `virtual` keyword before the `func`
keyword. A virtual function must be either public or protected, since private
members cannot be accessed even by derived classes, therefore making it
impossible to override the function and making it useless. A virtual function
may either have a default implementation, in which case the return type is
optinal like when defining a normal member function, or not provide an
implementation, in which case the return type must be specified. Virtual
functions that provided some default implementation may still be overriden. A
class deriving from another class which is also a derived class in itself may
override methods overriden by its parent class. To create an instance of a
class that involves any amount of inheritence in its declaration, it must
provide at least one implementation to all virtual functions, be it the default
implementation, an implementation provided by some parent class or an
implementation provided by the class itself.

```
class Base {
  virtual func public foo() => bool;
  virtual func public bar() => i32 { return 0; }
}

class A @ Base {
  override func public foo() => bool { return true; }
}

class B @ Base {
  override func public foo() => bool { return false; }
}
```

To make use of polymorphism, one of the core principals behind inheritence, the
object must wither be passed by reference or using a pointer type. The two
pointer types are `<>T` and `{}T`. The first is a unique pointer and the second
a shared pointer. A unique pointer is owning whereas a shared pointer can be
shared so that multiple objects point to the same data structure and only when
all shared pointers are deleted, the object itself is deleted. Pointers are the
way to allocate an object on the heap rather than on the stack.

```
func main([]string args, envs) => i32 {
  var polymorphicPtr : <>Base = A();
  polymorphicPtr.foo(); // returns true
  polymorphicPtr = B();
  polymorphicPtr.foo(); // returns false;

  var a = A();
  let b = B();

  var polymorphicRef : &Base;
  polymorphicRef = a;
  polymorphicRef.foo(); // returns true
  polymorphicPtr = b;
  polymorphicRef.foo(); // returns false
}
```

When a type has a virtual function, its destructor is implicitly made virtual
and the language runtime will implicitly create a chain from it and all derived
types' destructors, no `override` nor `virtual` needed.

When a class has other classes that derive from it, it is implicitly a reference
type. If a type has a field which is a refernece type, it is also a reference
type. If a type is recursive, i.e. it has a member which is a unique pointer of
the given type, or that one of its members has a member which is a unique
pointer of the given type, or one of their members... then the type is a
reference type. Shared pointers are ok, since "copying" them simply means
copying the address and adding one to the ref count, which are inexpensive tasks
compared to copying a unique pointer, which requires copying the actual object,
and since it is recursive, this may be very expensive (for example in the case
of a linked list). If copying the type and its members requires more than 4
moves of the biggest size available on the given architecture, the type is a
reference type. The maximum amount of moves that a value type may take up can be
configured when invoking the compiler.
