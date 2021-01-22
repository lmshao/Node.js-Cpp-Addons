//
// Created by Liming SHAO on 2021/01/19.
// Copyright 2021 Liming SHAO <lmshao@163.com>. All rights reserved.
//

#include "Clazz.h"
#include <sstream>

using node::AddEnvironmentCleanupHook;

v8::Persistent<v8::Function> Clazz::constructor;

void Clazz::Init(v8::Local<v8::Object> exports) {
    v8::Isolate *isolate = exports->GetIsolate();

    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);

#if NODE_MAJOR_VERSION > 12
    tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Clazz").ToLocalChecked());
#elif NODE_MAJOR_VERSION == 12
    tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Clazz", v8::NewStringType::kNormal).ToLocalChecked());
#else
    tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Clazz"));
#endif

    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "Add", Add);
    NODE_SET_PROTOTYPE_METHOD(tpl, "AllMembers", AllMembers);

#if NODE_MAJOR_VERSION >= 14
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    constructor.Reset(isolate, tpl->GetFunction(context).ToLocalChecked());
    exports->Set(context, v8::String::NewFromUtf8(isolate, "Clazz").ToLocalChecked(),
                 tpl->GetFunction(context).ToLocalChecked());
#elif NODE_MAJOR_VERSION == 12
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    constructor.Reset(isolate, tpl->GetFunction(context).ToLocalChecked());
    exports->Set(context, v8::String::NewFromUtf8(isolate, "Clazz", v8::NewStringType::kNormal).ToLocalChecked(),
                 tpl->GetFunction(context).ToLocalChecked());
#else
    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(v8::String::NewFromUtf8(isolate, "Clazz"), tpl->GetFunction());
#endif

#if NODE_MAJOR_VERSION >= 10
    node::AddEnvironmentCleanupHook(
      isolate,
      [](void *) {
          printf("in AddEnvironmentCleanupHook\n");
          constructor.Reset();
      },
      nullptr);
#else
    // An AtExit hook is a function that is invoked after the Node.js event loop has ended
    // but before the JavaScript VM is terminated and Node.js shuts down.
    // AtExit hooks are registered using the node::AtExit API.
    node::AtExit([](void *) { printf("in node::AtExit\n"); }, nullptr);
#endif
}

void Clazz::New(const v8::FunctionCallbackInfo<v8::Value> &args) {
    v8::Isolate *isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
#if NODE_MAJOR_VERSION >= 12
        std::string cName =
          args[0]->IsUndefined() ? "Undefined" : std::string(*v8::String::Utf8Value(isolate, args[0]));
#else
        std::string cName =
          args[0]->IsUndefined() ? "Undefined" : std::string(*v8::String::Utf8Value(args[0]->ToString()));
#endif

        Clazz *obj = new Clazz(cName);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());  // Return this object
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, constructor);
        v8::Local<v8::Object> result =
          cons->NewInstance(context, argc, argv).ToLocalChecked();  // Invoked as constructor: `new MyObject(...)`
        args.GetReturnValue().Set(result);
    }
}

void Clazz::Add(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Clazz *obj = ObjectWrap::Unwrap<Clazz>(args.Holder());

#if NODE_MAJOR_VERSION >= 10
    std::string mem = std::string(*v8::String::Utf8Value(args.GetIsolate(), args[0]));
#else
    std::string mem = std::string(*v8::String::Utf8Value(args[0]->ToString()));
#endif
    obj->_Add(mem);
    return;
}

void Clazz::AllMembers(const v8::FunctionCallbackInfo<v8::Value> &args) {
    v8::Isolate *isolate = args.GetIsolate();
    Clazz *obj = ObjectWrap::Unwrap<Clazz>(args.Holder());

    std::string res = obj->_AllMembers();
#if NODE_MAJOR_VERSION >= 14
    args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, res.c_str()).ToLocalChecked());
#else
    args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, res.c_str()));
#endif
}

Clazz::Clazz(std::string className) : _className(className) {}

Clazz::~Clazz() {
    printf("~Clazz()\n");
}

void Clazz::_Add(std::string member) {
    _members.insert(member);
}

std::string Clazz::_AllMembers() {
    std::ostringstream os;
    os << "Class " << _className << " members: ";
    int i = 1;
    for (auto m : _members) {
        os << i++ << '.' << m << ' ';
    }
    os << '.';
    return os.str();
}