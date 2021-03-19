## Lists & Maps
________________________________________________________________________________

A list is defined as a container type which holds multiple items of a given
type. It is defined using the `[T]` syntax, where `T` is the type of the objects
inside the list. Lists are treated as first class citizens in Regel and can
therefore be created using their own literals, the list literals. The list
literal composes `[[val0[, [val1[, [val2[...]]]]]]]` or in simpler terms, it is
enclosed using brackets and then values can be written inside, separated by
commas, and an optional comma may be placed after the last item, for example:

```
  let constList = ["hello", "world"]; // type is implicitly [string]
  var mutableList : [string];
  mutableList = ["bye"];
  mutableList += ["now"]; // invokes + operator of [string] with [string]
```

Items may also be consumed out of lists. This can only be done if the list is
mutable (declared using `var`). When consuming a list item, an exception to the
way consuming works is made; since it is impossible to always know at compile
time where accesses to the list will be made and if they are made after such an
item has been consumed from the list, a compile time error cannot be generated,
but in order to allow for consuming large items in lists, the value at that cell
is immediately set to the default value for the type. All built in types have a
predefined default value and therefore all other types also have a default
value which is simply having all its fields set to their default values. While
accesses to the list in places outside the function in which it is modified
can't always be traced, accesses to the list in the function in which the item
was consumed from it may be flagged with a warning and optionally cause an
compile time error.

Lists may hold a different amount of items at different points along the
execution. This means that lists may have other lists appended to them or just
items appended to them. Lists therefore are composed of three members, `size`,
`capacity` and some pointer to the data. This means that accesses to the data
have to go through that pointer. In reality, this shouldn't cause any noticeable
overhead since when optimizations are applied, the data pointer is accessed only
once and saved in some register for the duration of the accesses to the list and
then each access is much faster and does not require getting the data pointer
from the list each time. If this is not enough, arrays are here to save the day.
Unlike lists, arrays have a fixed size, which has to be known at compile time.
This size is encoded as part of the type signature of the array. To declare an
array rather than a list, just add the size to the type signature `[T, <size>]`
so for example to declare an array of `5` integers, the type signature could be
`[i32, 5]`. List literals (as shown in the example above) are as their name
indicates, lists. They can also be assigned to arrays instead. In case the
amount of items is lesser than the size of the array, the empty items are set
to the default value for the type. If there are more items than the array can
hold a compile time error is raised. If the amount of items in the list literal
(such as with complex list literals, discussed in their own chapter) cannot be
known at compile time, a compile time error is raised.

```
  let constArray : [i32, 5] = [0, 1, 2, 3, 4];
  var mutableArray : [i32, 10] = @constArray;
  mutableArray[5:] = [5, 6, 7, 8, 9];
  for let num in mutableArray {
    print(num); // prints 0 1 2 3 4 5 6 7 8 9
  } 
```

Another first class citizen container type in Regel is the map. Maps allow to
create tables from one type to another type. Maps are basically hash tables and
all built in types may serve as keys in a map. The map type is defined using
curly brackets `{TKey => TValue}`. A map literal can be written inside curly
brackets where the key is written first, then an arrow `=>` and then the value.
Pairs are separated by commas and the last pair may also have an optional comma
after it.

```
  let constMap = {'a' => 0xa, 'b' => 0xb, 'c' => 0xc};
  var mutableMap : {char => i32} = :constMap;
  mutableMap['d'..'g'] = 0xc..0x10;
```

Using a user defined type as a key in a map requires that type to have all its
fields be hashable. This is generally the case, where if a type only consists of
builtin types, it is implicitly hashable and a hash function is automatically
generated which uses the hashes for the built in types. In case the user
decides, a custom hash function may also be defined for the type which will then
be used, this may be useful if the type has some unique ID field which can be
used for faster hash calculations.
