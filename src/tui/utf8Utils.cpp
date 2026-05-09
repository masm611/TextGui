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

#include "utf8Utils.hpp"

#include "tui.hpp"

Utf8Char radioBtnUtf8Char0 = {0xE2, 0x97, 0x8B, 0};  // ○
Utf8Char radioBtnUtf8Char1 = {0xE2, 0x97, 0x8F, 0};  // ●

Utf8Char arrowBtnUtf8Char0 = {0xE2, 0x86, 0x90, 0};  // ←
Utf8Char arrowBtnUtf8Char1 = {0xE2, 0x86, 0x91, 0};  // ↑
Utf8Char arrowBtnUtf8Char2 = {0xE2, 0x86, 0x92, 0};  // →
Utf8Char arrowBtnUtf8Char3 = {0xE2, 0x86, 0x93, 0};  // ↓

uint32_t getPrintWidth(const std::string_view& str) {
    int width = 0;

    for (int i = 0; i < str.size(); ++i) {
        char c = str[i];

        if ((c & 0b10000000) == 0) {
            width++;
            continue;
        }

        int charSize = 0;
        switch (c & 0b11110000) {
            case 0b11000000:
                charSize = 2;
                break;
            case 0b11100000:
                charSize = 3;
                break;
            case 0b11110000:
                charSize = 4;
                break;
            default:
                throw std::invalid_argument("不正确的UTF-8序列");
        }

        Utf8Char u8c;
        for (int j = 0; j < charSize; ++j)
            u8c[j] = str[i + j];

        width += getPrintWidth(u8c);
        i += charSize - 1;
    }

    return width;
}

uint8_t getPrintWidth(const Utf8Char& u8c) {
    if ((u8c[0] & 0b10000000) == 0)
        return 1;

    if (u8c == radioBtnUtf8Char0 || u8c == radioBtnUtf8Char1)
        return Tui::instance()->getRadioBtnPrintWidth();
    else if (u8c == arrowBtnUtf8Char0 || u8c == arrowBtnUtf8Char1 || u8c == arrowBtnUtf8Char2 || u8c == arrowBtnUtf8Char3)
        return Tui::instance()->getArrowBtnPrintWidth();
    else
        return 2;
    // throw std::invalid_argument("不正确的UTF-8序列");
}

uint32_t getPrintWidth(const std::span<const Utf8Char>& u8cArray) {
    uint32_t width = 0;
    for (const auto& u8c : u8cArray) {
        width += getPrintWidth(u8c);
    }
    return width;
}

std::vector<Utf8Char> getUtf8Chars(const std::string_view& str) {
    std::vector<Utf8Char> chars;

    for (int i = 0; i < str.size();) {
        Utf8Char c;
        int charSize = 0;

        if ((str[i] & 0b10000000) == 0)
            charSize = 1;
        else
            switch (str[i] & 0b11110000) {
                case 0b11000000:
                    charSize = 2;
                    break;
                case 0b11100000:
                    charSize = 3;
                    break;
                case 0b11110000:
                    charSize = 4;
                    break;
                default:
                    throw std::invalid_argument("不正确的UTF-8序列");
            }

        for (int j = 0; j < charSize; ++j)
            c[j] = str[i + j];

        for (int j = charSize; j < 4; ++j)
            c[j] = 0;

        chars.push_back(c);
        i += charSize;
    }

    return chars;
}

std::string_view limitStrPrintWidth(const std::string_view& str, uint32_t width, uint32_t beginByte) {
    uint32_t curWidth = 0;
    for (int i = beginByte; i < str.size();) {
        char c = str[i];
        int charSize = 0;
        int charWidth = 0;

        if ((c & 0b10000000) == 0) {
            charSize = 1;
            charWidth = 1;
        } else
            switch (c & 0b11110000) {
                case 0b11000000:
                    charSize = 2;
                    charWidth = 2;
                    break;
                case 0b11100000:
                    charSize = 3;
                    charWidth = 2;
                    break;
                case 0b11110000:
                    charSize = 4;
                    charWidth = 2;
                    break;
                default:
                    throw std::invalid_argument("不正确的UTF-8序列");
            }

        if (curWidth + charWidth > width) {
            return str.substr(beginByte, i - beginByte);
        }

        curWidth += charWidth;
        i += charSize;
    }

    return str.substr(beginByte, str.end() - (str.begin() + beginByte));
}

std::string u8cArrayToString(const std::vector<Utf8Char>& u8cArray) {
    std::string str;
    for (auto& u8c : u8cArray)
        for (int i = 0; i < 4 && u8c[i] != 0; ++i)
            str += u8c[i];
    return str;
}

std::tuple<int, int> getMultiRowPrintSize(const std::string_view& str) {
    int maxWidth = 0;
    int height = 1;
    size_t lineStart = 0;

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\n') {
            int lineWidth = getPrintWidth(std::string_view(str.data() + lineStart, i - lineStart));
            if (lineWidth > maxWidth)
                maxWidth = lineWidth;
            ++height;
            lineStart = i + 1;
        }
    }

    int lastLineWidth = getPrintWidth(std::string_view(str.data() + lineStart, str.size() - lineStart));
    if (lastLineWidth > maxWidth)
        maxWidth = lastLineWidth;

    return {maxWidth, height};
}
