## Complex Hook Capture List
________________________________________________________________________________

As mentioned before in `Class Fields`, hook capture lists can get quite a bit
more complex than just binding to a name. To begin with the are group captures
such as `...`, `methods` and `fields`. Using a `,`, it is possible to have
multiple groups, so just like you can have multiple field names, you can just
as simply create a capture for all fields and a single method, name `foo` for
example and even exclude a field name `bar`.

```
class MyClass {
    pub var [a, b, c] : string;
    pub var bar : bool;

    hook [foo, fields, not bar] { 
        get {
            print("This is not printed when accessing doSomething or bar");
        }
    }

    pub func doSomething() {}
    pub func foo() {}
}
```

Say we wanted to implement a hook so that whenever we set the value of some
`i32` field of our class, we print the name of the field. One way to do this
would be to implement a `set` hook for each `i32` field to print the name. This
will result in a lot of code duplication. Complex hook capture lists to the
rescue! We can match hooks on types of fields such that we get a hook for all
`i32` fields.

```
class MyClass {
    pub var [a, b, c, d] : i32;
    hook [i32 as i32Field] { 
        set {
            print("Trying to set {} to {}", i32Field, value);
            value--;
            print("Will set {} to {} instead", i32Field, value);
        }
    }
}
```

Since we cannot have fields named the same as types that are exposed in the
current namespace, the compiler can know the matching is based on a type. The
`as` syntax allows as to capture groups and then access properties in the hook
itself. We also use the `value` keyword to get the value being set to the field.
We can also see the power of modifying the `value` being set to the field in the
above hook declaration.