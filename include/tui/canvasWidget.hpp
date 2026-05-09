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

#include "widget.hpp"
#include "pixel.hpp"
#include <vector>

class CanvasWidget : public Widget {
public:
    CanvasWidget(Widget* parent = nullptr);
    virtual ~CanvasWidget();

    void setPixel(short x, short y, const Pixel& pixel);
    void clearPixel(short x, short y);
    void setSize(short width, short height);
    void clear();

    virtual void draw() override;

    short canvasWidth() const { return _canvasWidth; }
    short canvasHeight() const { return _canvasHeight; }

    virtual Size getMinimumSize() const override { return Size(_canvasWidth, _canvasHeight); }

private:
    short _canvasWidth = 0;
    short _canvasHeight = 0;
    std::vector<Pixel> _pixels;
    std::vector<bool> _pixelSet;
};
