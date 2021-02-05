## User Defined Types
________________________________________________________________________________

To define a type, use the class keyword, followd by the name of the class and
then the class body in curly brackets. Types may have fields, which can either
be constant or mutable, depending on whether they are declared using the `let`
or `var` keyword, respectively.

The visibility of a member can be chosen using one of the three keywords:
`private`, `public` or `protected`. The default visibility is private. The same
visibility modifiers may be applied to constructors, which are also private by
default and to declare one, simply write a member function with the same name as
the class. A destructor may be written using a `!` before the name of the class.
There may only be one destructor and it is de facto private and may only be
called by the language implementation sees it neccesary.

```
class MyClass {
  let a : i32 private;

  // protected member
  var b : i32 protected;

  // public member
  let c : i32 public;

  // a public default constructor
  func public MyClass() {}

  // a destructor must be written as private
  func !MyClass() {}

  // a public member function
  func public add(i32 a, b) => i32 { return a + b; }
}
```

When defining a member function, it cannot modify the object it is bound to by
default. To enable modifying the object, the method must be declared using the
`&` modifier, where the syntax is `func [visibility-modifier]
[mutability-modifier] <identnfier>([args...]) [=> <ret-type>] <method-body>`.
The mutability modifier may also be one of `:` or `@` for either a consuming or
a copying modifier. When calling the method, the invocation must aggree with the
way the method was defined, such that:

```
class Vector3f {
  // ...

  func public &extend(Vector3f other) {
    x, y, z += other.[x, y, z];
  }
}

func main([string] args, envs) => i32 {
  var vec1 : Vector3f = ...;
  let vec2 : Vector3f = ...;

  // the invocation aggrees with the declaration, this is dones to make sure
  // that by simply looking at the call site, it is possible to know if a member
  // function will change the underlying object, consume it or even if a copy is
  // made when invoking the member function
  vec1.&extend(vec2);
}
```
