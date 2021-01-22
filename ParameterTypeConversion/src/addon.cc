//
// Created by Liming SHAO on 2021/01/19.
// Copyright 2021 Liming SHAO <lmshao@163.com>. All rights reserved.
//

#include <node.h>

using namespace v8;

void passInt32(const FunctionCallbackInfo<Value> &args){
    int value = args[0]->Int32Value();
    args.GetReturnValue().Set(value);
}

void passUInt32(const FunctionCallbackInfo<Value> &args){
    uint32_t value = args[0]->Uint32Value();
    args.GetReturnValue().Set(value);
}

void passInt64(const FunctionCallbackInfo<Value> &args){
    // int64_t value = args[0]->IntegerValue();
    // args.GetReturnValue().Set(Value::Cast(value));
    args.GetReturnValue().Set(args[0]);
}

void passDouble(const FunctionCallbackInfo<Value> &args){
    double value = args[0]->NumberValue();
    args.GetReturnValue().Set(value);
}

void passBool(const FunctionCallbackInfo<Value> &args){
    bool value = args[0]->BooleanValue();
    args.GetReturnValue().Set(value);
}

void passString(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();
    std::string value = std::string(*String::Utf8Value(isolate, args[0]));
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, value.c_str()));
}

void passObject(const FunctionCallbackInfo<Value> &args){
    int value = args[0]->Int32Value();
    args.GetReturnValue().Set(value);
}

// for storing the callback function pointer
v8::Persistent<v8::Object> gStore = { Isolate::GetCurrent(), Object::New(Isolate::GetCurrent()) };

void toJson(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    const unsigned argc = args.Length();
    Local<Value> argv[argc] = {};
    for (size_t i = 0; i < argc; i++) {
        argv[i] = args[i];
    }

    // get the function pointer as callback function
    auto store = Local<Object>::New(isolate, gStore);
    auto val = store->Get(String::NewFromUtf8(isolate, "getJson"));
    // invoke the callback function
    Local<Value> jsResult = Local<Function>::Cast(val)->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    // return the result
    args.GetReturnValue().Set(jsResult);
}

void passFunction(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (!args[0]->IsFunction()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
        return;
    }

    // store the function pointer as callback function
    Local<Object>::New(isolate, gStore)->Set(String::NewFromUtf8(isolate, "getJson"), args[0]);

    // return a c++ function
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, toJson);
    Local<Function> fn = tpl->GetFunction();
    args.GetReturnValue().Set(fn);
}

void Initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "passInt32", passInt32);
    NODE_SET_METHOD(exports, "passUInt32", passUInt32);
    NODE_SET_METHOD(exports, "passInt64", passInt64);
    NODE_SET_METHOD(exports, "passDouble", passDouble);
    NODE_SET_METHOD(exports, "passBool", passBool);
    NODE_SET_METHOD(exports, "passString", passString);
    NODE_SET_METHOD(exports, "passObject", passObject);
    NODE_SET_METHOD(exports, "passFunction", passFunction);
}

NODE_MODULE(mylib, Initialize)