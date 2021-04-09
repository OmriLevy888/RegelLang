## Passing Arguments
________________________________________________________________________________

Passing an argument can be a bit tricky, since it requires knowledge of how
types work in Regel. Types fall into one of two categories; `value` types and
`reference` types. Value types are categorized as generally small, simple types,
which many builtin types are. These types include `i32`, `float`, `char` and
more. Reference types are more complex types, such that copying them around is
considered an expensive operation. These types include `string` and vectors such
as `[i32]`.

When creating a function, there are multiple ways to define the type of an
argument. A function can choose between one of 4 different behaviors:

```
func foo(i32 a) {}  // 1
func foo(&i32 a) {} // 2
func foo(:i32 a) {} // 3
```

Declaration `1` uses `i32`. Since `i32` is a value type, the value is copied,
which is a simply move instruction and the function may not change the value of
a. If rather than `i32`, a reference type were used, the object would be passed
by referenced rather than copied but the function would not be able to change
its value. This is the default way to encourage as much usage of constants which
allow for more optimizations by the compiler and to avoid unnecessary copies and
moves.

Declaration `2` uses `&i32`. This means the underlying object is passed by
reference and may be changed, and any changes will affect the object passed to
the function.

Declaration `3` uses `:i32`. This is a bit different from the other ways of
declaring an argument. This means the argument is consuming. The function may do
whatever it wants with the object, even let other functions consume it. When an
object is consumed, it is no longer valid to use it. The exception is when
moving an item out of a vector. In that case, the cell in which the item was in
the vector is reset to the default value for the type.

When invoking one of the above declarations, the way the argument is passed must
aggree with the declaration:

```
func main([string] args, envs) {
  a := 5;
  foo(a);  // invokes 1
  foo(&a); // invokes 2
  foo(:a); // invokes 3
  foo(@a); // also invokes 3
}
```

In the last invocation, the `@` prefix is used. It creates a copy of `a` and
passes it to the function. Since the copy is not bound to any variable or
constant, it is passed to the consuming function. If a consuming overload is not
defined, it would be passed to the default overload (the one that takes `i32`).
If neither is defined, a compiler error is generated.