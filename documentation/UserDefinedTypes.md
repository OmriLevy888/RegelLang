## User Defined Types
________________________________________________________________________________

To define a type, use the class keyword, followed by the name of the class and
then the class body in curly brackets. Types may have fields, which can either
be constant or mutable, depending on whether they are declared using the `var`
or not.

The visibility of a member can either be private or public. To make a member
public, use the `pub` modifier, otherwise the member is private. The same
visibility modifiers may be applied to constructors, which are also private by
default and to declare one, simply write a member function with the same name as
the class. A destructor may be written using a `!` before the name of the class.
There may only be one destructor and it is de facto private and may only be
called by the language implementation when sees it necessary.

```
class MyClass {
  // a private member
  a : i32;

  // public member
  pub b : i32;

  // a public default constructor
  pub func MyClass() {}

  // a destructor must be written as private
  func !MyClass() {}

  // a public member function
  pub func add(i32 a, b) => i32 { return a + b; }
}
```

A constant field in a class may only be assigned to in a constructor, though it
may be assigned to multiple times during the constructor. In essence, it is
marked mutable in constructors but immutable for all other contexts.

When defining a member function, it cannot modify the object it is bound to by
default. To enable modifying the object, the method must be declared using the
`&` modifier, where the syntax is `[pub] func [mutability-modifier]
<identnfier>([args...]) [=> <ret-type>] <method-body>`.
The mutability modifier may also be `:` for a consuming modifier. When calling
the method, the invocation must agree with the way the method was defined, such that:

```
class Vector3f {
  ...

  pub func &extend(Vector3f other) {
    x, y, z += other.[x, y, z];
  }
}

func main([string] args, envs) => i32 {
  var vec1 : Vector3f = ...;
  let vec2 : Vector3f = ...;

  // the invocation agrees with the declaration, this is done to make sure that
  // by simply looking at the call site, it is possible to know if a member
  // function will change the underlying object, consume it or even if a copy is
  // made when invoking the member function
  vec1.&extend(vec2);
}
```

Classes may also be extended, which is done using an `extends` block. Only
functions can be defined inside an `extends` block and the block may be defined
a different namespace than the class itself, making it a great tool for
separating functions that implement different interfaces to different files.

```
class Person {
  pub name : string;
  pub age : i32;
}

extend Person {
  pub greet() {
    print("I am {} and I am {} years old", name, age);
  }
}
```