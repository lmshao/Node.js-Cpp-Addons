//
// Created by Liming SHAO on 2021/01/19.
// Copyright 2021 Liming SHAO <lmshao@163.com>. All rights reserved.
//

#include <node.h>

using namespace v8;

void sayHello(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

#if NODE_MAJOR_VERSION >= 14
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "Hello World!").ToLocalChecked());
#else
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "Hello World!"));
#endif
}

// exports.Hello = Hello;
void Initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "Hello", sayHello);
}

// module.exports = Hello;
void Initialize2(Local<Object> exports, Local<Object> module) {
    NODE_SET_METHOD(module, "exports", sayHello);
}

// Note:
// 1. There is no semi-colon after NODE_MODULE as it's not a function (see node.h).
//    NODE_MODULE()后面没有分号，因为它不是一个函数
// 2. The module_name must match the filename of the final binary (excluding the .node suffix).
//    官方文档说模块名称必须与最终二进制文件的文件名匹配(不包括.node后缀)，不过不匹配好像也没问题
NODE_MODULE(hello, Initialize)