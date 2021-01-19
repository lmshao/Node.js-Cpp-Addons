const mylib = require('./build/Release/mylib.node');

// 1. (n) => {return f(n);}
console.log('f(10) =', mylib.Fibonacci(10)); // f(10) = 55

// 2. (n, callback) => { callback(f(n)); }
mylib.Fibonacci_Callback(10, (result) => {
    console.log('f(10) =', result); // f(10) = 55
});