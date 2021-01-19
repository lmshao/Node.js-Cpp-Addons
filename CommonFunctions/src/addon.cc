//
// Created by Liming SHAO on 2021/01/19.
// Copyright 2021 Liming SHAO <lmshao@163.com>. All rights reserved.
//

#include <node.h>

using namespace v8;

// Fibonacci sequence
int f(int n) {
    return (n < 3) ? 1 : f(n - 1) + f(n - 2);
}

void Fibonacci(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    // Check the number of arguments passed.
    if (args.Length() != 1) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }

    // Check the argument types
    if (!args[0]->IsInt32()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
        return;
    }

    int n = args[0]->Int32Value();
    int res = f(n);
    args.GetReturnValue().Set(res);
}

void Fibonacci_Callback(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    // Check the number of arguments passed.
    if (args.Length() != 2) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }

    // Check the argument types
    if (!args[0]->IsInt32() || !args[1]->IsFunction()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
        return;
    }

    int n = args[0]->Int32Value();
    int res = f(n);

    Local<Function> cb = Local<Function>::Cast(args[1]);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { Number::New(isolate, res) };
    cb->Call(Null(isolate), argc, argv);
}

// exports.Fibonacci = Fibonacci;
// exports.Fibonacci_Callback = Fibonacci_Callback;
void Initialize(Handle<Object> exports) {
    NODE_SET_METHOD(exports, "Fibonacci", Fibonacci);
    NODE_SET_METHOD(exports, "Fibonacci_Callback", Fibonacci_Callback);
}

// Note:
// 1. There is no semi-colon after NODE_MODULE as it's not a function (see node.h).
//    NODE_MODULE()后面没有分号，因为它不是一个函数
// 2. The module_name must match the filename of the final binary (excluding the .node suffix).
//    官方文档说模块名称必须与最终二进制文件的文件名匹配(不包括.node后缀)，不过不匹配好像也没问题
NODE_MODULE(mylib, Initialize)