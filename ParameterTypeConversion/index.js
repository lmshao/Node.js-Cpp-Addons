const mylib = require('./build/Release/mylib.node');

console.log(mylib.passInt32(-1));

console.log(mylib.passUInt32(4294967295));

console.log(mylib.passInt64(-1));

console.log(mylib.passDouble(-1.23));

console.log(mylib.passBool(false));

console.log(mylib.passString('Hello'));

let getJson = (_name, _age, _addr) => {
    let info = { name: _name, age: _age, addr: _addr};

    return JSON.stringify(info);
}

// pass 'getJson' as a callback function
let CppFunction = mylib.passFunction(getJson);
console.log(CppFunction('Tom', 12));
console.log(CppFunction('Jim', 15, "Beijing"));