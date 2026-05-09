/*
Copyright 2026 masm611 <2838105183@qq.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#include "color.hpp"
#include "utf8Utils.hpp"

struct Pixel {
    Color foregroundColor = Color(255, 255, 255);
    Color backgroundColor = Color(0, 0, 0);

    Utf8Char character = {' ', 0, 0, 0};  // character[0]==0 的时候意味着前一个字符是全角，这时候不要显示该字符
};