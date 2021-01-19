# Node.js-Cpp-Addons
Call C++ function from Node.js

Reference: [Node.js Documentation C++ Addons](https://nodejs.org/docs/latest-v8.x/api/addons.html)

## Install node-gyp
```sh
npm install -g node-gyp
```

## Build C++ code
```sh
node-gyp configure build
// node-gyp configure
// node-gyp build
```

## Examples

### HelloWorld

Just return a string "Hello World!" from C++.

## CommonFunctions

A function to calculate Fibonacci sequence with:
- Passing arguments
- Throwing exceptions
- Callbacks

## CppClassWrapper

`Add` some students to a class `(Clazz)`, and finally show who are in the class.