//
// Created by Liming SHAO on 2021/01/19.
// Copyright 2021 Liming SHAO <lmshao@163.com>. All rights reserved.
//

#include <node.h>
#include "Clazz.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
    Clazz::Init(exports);
}

NODE_MODULE(hello, InitAll)