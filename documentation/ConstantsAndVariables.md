## Constants & Variables
________________________________________________________________________________

In Regel, there are two ways to make a declaration, either using the `let`
keyword or the `var` keyword. Using the `let` keyword indicates a constant
whereas the `var` keyword indicates a variable. After the declaration keyword
comes and identifier to bind the declaration to. Identifiers in Regel may start
with either a letter or an underscore and then contain any combination of
letters, underscores and decimal digits.

```
let a = 5; // implicit type
var b : i16 = 5; // explicit type
```

By default a type does not have to be specified unless it would otherwise be
ambigious (see example in Conditionals) but can be specified when adding a colon
after the identifier and then the type. `5` has the implicit type `i32` where as
in the `var` declaration, the explicit specification of `i16` forces 5 to be
truncated to 16 bits. Types can also be supplied using sufixes, so instead of
declaring `b` to be of type `i16`, it is possible to force `5` to be of type
`i16` by postfixing it with the type, therefore writing `5i16`. The built in
sufixes are

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
