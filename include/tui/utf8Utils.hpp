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

#include <string>
#include <cstdint>
#include <array>
#include <span>
#include <tuple>

#include <vector>

typedef std::array<uint8_t, 4> Utf8Char;

uint32_t getPrintWidth(const std::string_view& str);
uint8_t getPrintWidth(const Utf8Char& u8c);
uint32_t getPrintWidth(const std::span<const Utf8Char>& u8cArray);
std::vector<Utf8Char> getUtf8Chars(const std::string_view& str);
std::string_view limitStrPrintWidth(const std::string_view& str, uint32_t width, uint32_t beginByte = 0);
std::string u8cArrayToString(const std::vector<Utf8Char>& u8cArray);

std::tuple<int, int> getMultiRowPrintSize(const std::string_view& str);

extern Utf8Char radioBtnUtf8Char0;  // ○
extern Utf8Char radioBtnUtf8Char1;  // ●

extern Utf8Char arrowBtnUtf8Char0;  // ←
extern Utf8Char arrowBtnUtf8Char1;  // ↑
extern Utf8Char arrowBtnUtf8Char2;  // →
extern Utf8Char arrowBtnUtf8Char3;  // ↓