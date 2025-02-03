# eta-lang(.n)

an interpreted toy language built for fun!
this project is currently in alpha stages.

# build

```bash
meson setup build
cd build
ninja

#for repl
./eta
#for file input
./eta <file-name>
```

# syntax

## varibale declaration and assignment

```
# declaration
let number = 123;
let decimal = 56.7;
let a_bool_variable = true;
let string_1 = "Hello, World!";

# reassignment
number = 145;

# this will cause an error,
# as types cannot be changed after declaration
number = 125.5;
number = int(125.5); # this is okay!
```

## branch

```
if (number > 100) {
  number += 10;
} else {
  number -= 10;
}

# sadly there is no "else if" as of now
# but check this out
let new_var = if(number > 100) { number } else { "hello!!" }
```

## for

```
# currently "for" only supports
# one statement in the (;;) per blocks
# and the first block can be used for "let" statement only
for(let i = 0; i < 10; i += 1) { # no ++ as of now
  //more code
}
```

## function

```
# a varibale binded to a function
# cannot be reassigned
let incrementer = fn (n) {
  return n + 1;
}

let incrementer = fn() {} # this will cause an error
```

## arrays

```
let arr = []; # this is an array

# arrays in eta-lang are of mixed types
push(arr, 0);
push(arr, incrementer);
push(arr, "Hello, World");

# arrays by default can only
# be copied by refernce when assigned to a new variable
let arr_2 = arr;

# so this will mutate arr_2 too
push(arr_2, 100);

# the solution! > slice copies the arr's content
let arr_3 = slice(arr);
push(arr_3); # this won't mutate arr
```

## inbuilt functions

- print(...): `prints elements to the console`
- println(...): `same as print() but shift the cursor to new line`
- len(...): `get the length for string and array type`
- type(...): `returns the type in string format, ex: "int", "float", "string", "bool", "function"`
- push(...): `pushes an element into an array`
- pop(...): `pop an element from array`
- pop(...): `pop an element from array`
- slice(...): `can be used to copy a array by value or to slice an array, ex: slice(arr, 0, 2)`
- int(...): `typecasts to int`
- float(...): `typecasts to float`
- any(...): `used to intialize variable whose type is not known at declaration time`
