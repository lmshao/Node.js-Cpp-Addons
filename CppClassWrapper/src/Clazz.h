//
// Created by Liming SHAO on 2021/01/19.
// Copyright 2021 Liming SHAO <lmshao@163.com>. All rights reserved.
//

#ifndef CLAZZ_H_
#define CLAZZ_H_

#include <node.h>
#include <node_object_wrap.h>

#include <set>
#include <string>

class Clazz : public node::ObjectWrap
{
  public:
    static void Init(v8::Local<v8::Object> exports);

  private:
    static void New(const v8::FunctionCallbackInfo<v8::Value> &args);

    // wrapper
    static void Add(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void AllMembers(const v8::FunctionCallbackInfo<v8::Value> &args);

    explicit Clazz(std::string className);
    ~Clazz();

    // original functions
    void _Add(std::string member);
    std::string _AllMembers();

    static v8::Persistent<v8::Function> constructor;

    std::set<std::string> _members;
    std::string _className;
};

#endif  // CLAZZ_H_