const mylib = require('./build/Release/mylib.node');

const clazz = new mylib.Clazz("Chinese");
// const clazz = mylib.Clazz("Chinese");
clazz.Add('Tom');
clazz.Add('Mary');
clazz.Add('Liming');
console.log(clazz.AllMembers()); // Class Math: Liming Mary Tom .