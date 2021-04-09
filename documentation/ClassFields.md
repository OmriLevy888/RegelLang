## Class Fields
________________________________________________________________________________

Imagine implementing a `Vector3f` class. A typical implementation would look
something like this:

```
// 1
class Vector3f {
    pub var x : float;
    pub var y : float;
    pub var z : float;
}
```

Just this simply declaration repeats itself a lot. Therefore Regel allows for
some shorthands to avoid repeating yourself. The following declarations are
equivalent to the one above.

```
// 2
class Vector3f {
    pub var x : float, y : float, z : float;
}

// 3
class Vector3f {
    pub var [x, y, z] : float;
}
```

The syntax in `3` is by far the shortest and best for a case like this. `2` is
also useful and may come in handy when each of the fields has a different type
but it they must all have the same visibility and all be either a variable or
a constant field.

Now what if we wanted to have our vector class calculate a magnitude and even
cache it so that it is only calculated once until the vector is changed for
some reason. A naive implementation would be to add two variable fields, a
`bool` to indicate whether the magnitude is valid and another for the magnitude
and use `set` functions for `x`, `y` and `z` to set the validity of the
magnitude and a `get` function for the magnitude which checks the validity and
calculates the new magnitude if needed. This will in result in a lot of code
for something fairly trivial and the repetition of the `set` methods is bad,
what if we then want to make this a vector with 4 dimension and forget to add
another `set` method, or rather forget to use the `set` method for one of the
fields? In languages such as C++ you would make `x`, `y` and `z` private and
also add `get` methods to them to make sure all access has to go through the
`get` and `set` methods. This will result in even more code...

Regel has multiple ways to make situations like this simpler to code and
maintain. The first is rather similar to the C++ solution, just integrated in
the syntax of the language, think of it like C# properties.

```
// 4
class Vector3f {
    pub var x : { set { cachedMagnitude = false; x = value; }, get { return x; }}
    pub var y : { set { cachedMagnitude = false; y = value; }, get; }
    pub var z : { set { cachedMagnitude = false; z = value; }, get; }

    var cachedMagnitude : bool;
    pub var magnitude : float {
        get { 
            if not cachedMagnitude {
                magnitude = ((x ** 2) + (y ** 2) + (z ** 2)) ** 0.5f;
                cachedMagnitude = true;
            }
            return magnitude;
        } 
    }
}
```

This already has a few advantages over the C++ version, but it also has a few
disadvantages. To start with the advantages, `x`, `y`, `z` and `magnitude` all
have the right visibility, the only private field is `cachedMagnitude` which
makes sense since it is part of some inner mechanism related to how `Vector3f`
works and is invisible to anyone using the class itself. The disadvantage this
method has compared to the C++ code is that like C# properties, when looking
reading a piece of code that fetches `magnitude` or sets one of the other public
fields, unless you know how `Vector3f` was declared, you cannot know that there
is possibly a function call being made (if the code was not inlined by the
compiler). It does however provide us with a way to modify behavior and add
behavior, such as this cache behavior after already using the class with no
`get` or `set` functions without causing any code using the class to no longer
compile.

The code above shows that getters can either be defined explicitly, or simply
by using the `get` keyword to indicate we want the default behavior. If no
implementation is provided and the `get` keyword is omitted, the field only
provides `set` access.

Still, even with this syntax, there is a lot of repetition in `4`. Regel also
addresses this issue with its syntax.

```
// 5
class Vector3f {
    pub var [x, y, z] : float;
    hook [x] { set { cachedMagnitude = false; } }
    hook [y] { set { cachedMagnitude = false; } }
    hook [z] { set { cachedMagnitude = false; } }

    var cachedMagnitude : bool;
    pub var magnitude : float {
        get {
            if not cachedMagnitude {
                magnitude = ((x ** 2) + (y ** 2) + (z ** 2)) ** 0.5f;
            }
            return magnitude;
        }
    }
    [magnitude] { get { cachedMagnitude = true; } }
```

The new syntax introduced here is referred to as `on access hooks`. These block
can be used to add behavior when accessing a field of the class or even when
accessing a method. Inside a hook block, both `get` and `set` hooks can be
defined. A `set` hook is executed before the `set` logic and a `get` hook is
executed after the `get` logic. Both have access to the `value` keyword and may
even modify it to change the value set to the field or returned from the access
to the field.

Even with this new syntax, the code still repeats itself quite a lot. Luckily,
Regel even addresses this issue.

```
// 6
class Vector3f {
    pub var [x, y, z] : float;
    hook [x, z, z] { set { cachedMagnitude = false; } }

    var cachedMagnitude : bool;
    pub var magnitude : float {
        get {
            if not cachedMagnitude {
                magnitude = ((x ** 2) + (y ** 2) + (z ** 2)) ** 0.5f;
                cachedMagnitude = true;
            }
            return magnitude;
        }
    }
}
```

And here we have our final version, no obvious repetition in our code. Just like
we can use multiple names in a `hook`'s capture list, we can also use the `...`
syntax to apply the hook to all members of a type which can be quite useful to
create logging behavior. We can also use either the `[methods]` or the
`[fields]` capture lists to indicate a hook to run over all methods or over
all fields, more information on the chapter on complex hook capture lists.

Also note how `cachedMagnitude = true` moved back into the getter of
`magnitude`. This is done purely for performance reasons. When using the `get`
hook to set the value fo `cachedMagnitude`, we basically set it to `true` even
if we did not calculate `magnitude` again which is not necessary. 