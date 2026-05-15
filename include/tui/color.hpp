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

#include <cstdint>
#include <stdexcept>

class Color {
public:
    Color() : rgb{0, 0, 0} {}

    Color(uint8_t r, uint8_t g, uint8_t b) : rgb{r, g, b} {}

    Color(Color&& other) noexcept {
        rgb[0] = other.rgb[0];
        rgb[1] = other.rgb[1];
        rgb[2] = other.rgb[2];
    }

    Color(const Color& other) {
        rgb[0] = other.rgb[0];
        rgb[1] = other.rgb[1];
        rgb[2] = other.rgb[2];
    }

    Color& operator=(const Color& other) {
        if (this != &other) {
            rgb[0] = other.rgb[0];
            rgb[1] = other.rgb[1];
            rgb[2] = other.rgb[2];
        }
        return *this;
    }

    uint8_t r() const { return rgb[0]; }
    uint8_t g() const { return rgb[1]; }
    uint8_t b() const { return rgb[2]; }

    void setR(uint8_t r) { rgb[0] = r; }
    void setG(uint8_t g) { rgb[1] = g; }
    void setB(uint8_t b) { rgb[2] = b; }
    void setRGB(uint8_t r, uint8_t g, uint8_t b) {
        rgb[0] = r;
        rgb[1] = g;
        rgb[2] = b;
    }

    uint8_t operator[](size_t index) const {
        if (index >= 3) {
            throw std::out_of_range("Color index out of range");
        }
        return rgb[index];
    }

    bool operator==(const Color& other) const {
        return rgb[0] == other.rgb[0] && rgb[1] == other.rgb[1] && rgb[2] == other.rgb[2];
    }

private:
    uint8_t rgb[3] = {0, 0, 0};
};
