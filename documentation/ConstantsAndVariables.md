## Constants & Variables
________________________________________________________________________________

In Regel, there are two ways to make a declaration, either using the `var`
keyword or none at all. Using no keyword indicates a constant whereas the `var`
keyword indicates a variable. Then comes and identifier to bind the declaration
to. Identifiers in Regel may start with either a letter or an underscore and
then contain any combination of letters, underscores and decimal digits.

```
a := 5; // implicit type
var b : i16 = 5; // explicit type
```

To differentiate assignments from initializations, the `:=` syntax is used. Note
that unlike some other languages, this is not an operator; the colon (`:`) and
the equal sign (`=`) may be separated. When separated, the type comes between
them, otherwise the type is inferred from the expression assigned during the
declaration. It is considered a best practice to keep them together when there
is no explicit type for consistency's sake.

Another possibility is to declare a variable but not to bind any value to it.
This is not done often since it keeps the variable uninitialized and is
therefore dangerous. When the variable is uninitialized, Regel makes no promise
of what its value may be. The syntax to declare an uninitialized constant or
variable is simply to omit the `=` and provide no value. An explicit type has to
be provided since there is no expression to derive the type from. Constant values
may not be declared uninitialized.

```
// declared up here
var a : i32;

// this is dangerous, the compiler will try to generate errors when it can know
// for sure an access is made to an uninitialized variable or a warning when it
// isn't sure. in debug builds, the compiler will also generate code to check
// whether the item is initialized during run time to help catch such bugs. this
// code will not be present in release builds (can also be configured to be set
// off in debug builds)
print(a);

// initialized here
a = 5;
```

By default a type does not have to be specified unless it would otherwise be
ambiguous (see example in Conditionals) but can be specified when adding a colon
after the identifier and then the type. `5` has the implicit type `i32` where as
in the `var` declaration, the explicit specification of `i16` forces 5 to be
truncated to 16 bits. Types can also be supplied using suffixes, so instead of
declaring `b` to be of type `i16`, it is possible to force `5` to be of type
`i16` by postfixing it with the type, therefore writing `5i16`. The built in
suffixes are

| size    | signed integer | unsigned integer | floating point |
| ------- | -------------- | ---------------- | -------------- |
| 8 bits  | i8             | u8               |                |
| 16 bits | i16            | u16              |                |
| 32 bits | i/i32          | u/u32            | f              |
| 64 bits | i64            | u64              |                |

There is no sufix for 64 bit floating point literals, since it is the
default type for floating point literals. i/i32 and u/u32 mean the same
respectively. The default types for integer literals is i32.

Other built in literals include the `true` and `false` keywords for `bool`
literals, `char` literals, which are single character in size, encapsulated
by single quotes (') and may have the `uc` postfix to indicate unicode or when
using the `\u` escape character in the literal and the `string` literal,
encapsulated by double quotes (") which may also have the same unicode
properties as the `char` literal.
