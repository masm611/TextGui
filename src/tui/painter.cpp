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

#include "painter.hpp"

#include "tui.hpp"
#include "utf8Utils.hpp"

Painter::Painter(Widget* widget) {
    Tui* tui = Tui::instance();
    if (tui == nullptr)
        throw std::runtime_error("Painter构造失败：Tui实例不存在");

    this->canvas = tui->getCanvas();
    if (this->canvas == nullptr)
        throw std::runtime_error("Painter构造失败：Tui未初始化canvas实例");

    Widget* parentWidget = widget->getParent();
    if (parentWidget == nullptr) {
        drawableArea = widget->geometry();

        drawableArea.width = std::min<short>(drawableArea.width, canvas->width() - drawableArea.x);
        drawableArea.height = std::min<short>(drawableArea.height, canvas->height() - drawableArea.y);
    } else {
        Painter parentPainter(parentWidget);
        drawableArea = parentPainter.drawableArea;

        Rect childDrawableArea = parentWidget->getChildDrawableArea(widget);
        Rect childGeometry = widget->geometry();
        drawableArea.x += childDrawableArea.x + childGeometry.x;
        drawableArea.y += childDrawableArea.y + childGeometry.y;

        // 不能这么写，要不然不能做出子窗口左上超出父窗口的效果
        // // 左上角不能超出父painter的drawableArea
        // if (drawableArea.x < parentPainter.drawableArea.x)
        //     drawableArea.x = parentPainter.drawableArea.x;
        // if (drawableArea.y < parentPainter.drawableArea.y)
        //     drawableArea.y = parentPainter.drawableArea.y;

        // 不能超出childDrawableArea
        drawableArea.width = std::min<short>(childDrawableArea.width - childGeometry.x, childGeometry.width);
        drawableArea.height = std::min<short>(childDrawableArea.height - childGeometry.y, childGeometry.height);

        // 也不能超出父painter的区域
        short parentDrawableAreaEndX = drawableArea.x + drawableArea.width;
        short parentDrawableAreaEndY = drawableArea.y + drawableArea.height;
        drawableArea.width = std::min<short>(parentDrawableAreaEndX - drawableArea.x, drawableArea.width);
        drawableArea.height = std::min<short>(parentDrawableAreaEndY - drawableArea.y, drawableArea.height);

        // 宽高不能小于0
        drawableArea.width = std::max<short>(drawableArea.width, 0);
        drawableArea.height = std::max<short>(drawableArea.height, 0);
    }

    realDrawableArea = getRealDrawableArea(widget);

    this->widget = widget;
}

Painter::~Painter() {
}

void Painter::setPixel(short x, short y, Pixel pixel) {
    if (x < 0 || (((pixel.character[0] & 0b10000000) == 0) ? x : x + 1) >= drawableArea.width || y < 0 || y >= drawableArea.height)
        return;

    if (drawableArea.x + x < 0 || drawableArea.x + x >= canvas->width() || drawableArea.y + y < 0 || drawableArea.y + y >= canvas->height())
        return;

    if (x + widget->geometry().x < 0 || y + widget->geometry().y < 0)
        return;

    // // 判断是否超出父窗口范围
    // auto parentWidget = widget->getParent();
    // if (parentWidget != nullptr) {
    //     Painter parentPainter(parentWidget);
    //     if (drawableArea.x + x < parentPainter.drawableArea.x || drawableArea.x + x >= parentPainter.drawableArea.x + parentPainter.drawableArea.width || drawableArea.y + y < parentPainter.drawableArea.y || drawableArea.y + y >= parentPainter.drawableArea.y + parentPainter.drawableArea.height)
    //         return;
    // }

    // realDrawableArea判断
    if (drawableArea.x + x < realDrawableArea.x || drawableArea.x + x >= realDrawableArea.x + realDrawableArea.width || drawableArea.y + y < realDrawableArea.y || drawableArea.y + y >= realDrawableArea.y + realDrawableArea.height)
        return;
    if (getPrintWidth(pixel.character) > 1 && drawableArea.x + x + 1 >= realDrawableArea.x + realDrawableArea.width)
        return;
    if (pixel.character[0] == 0 && drawableArea.x + x - 1 < realDrawableArea.x) {
        pixel.foregroundColor = pixel.backgroundColor;
        pixel.character = {' ', 0, 0, 0};
    }

    auto [canvasWidth, canvasHeight] = canvas->getSize();

    // 原始canvas的最右下不给显示全角字符，屏幕会闪烁
    if (drawableArea.x + x >= canvasWidth - 1 && drawableArea.y + y >= canvasHeight - 1 && getPrintWidth(pixel.character) > 1)
        return;

    if (drawableArea.x + x >= canvasWidth || drawableArea.y + y >= canvasHeight)
        return;

    // 压住原有全角字符处理
    Pixel originPixel = canvas->pixelArray()[(drawableArea.y + y) * canvas->width() + drawableArea.x + x];
    if (originPixel.character[0] == 0 && drawableArea.x + x - 1 >= 0)
        canvas->pixelArray()[(drawableArea.y + y) * canvas->width() + drawableArea.x + x - 1].character = {' ', 0, 0, 0};
    else if ((originPixel.character[0] & 0b10000000) != 0 && drawableArea.x + x + 1 < canvas->width())
        canvas->pixelArray()[(drawableArea.y + y) * canvas->width() + drawableArea.x + x + 1].character = {' ', 0, 0, 0};

    canvas->pixelArray()[(drawableArea.y + y) * canvas->width() + drawableArea.x + x] = pixel;

    // 0字符显示在最左边处理
    if (pixel.character[0] == 0 && x + widget->geometry().x == 0)
        canvas->pixelArray()[(drawableArea.y + y) * canvas->width() + drawableArea.x + x].character = {' ', 0, 0, 0};

    // 0字符显示在屏幕最左边处理
    if (pixel.character[0] == 0 && drawableArea.x + x == 0)
        canvas->pixelArray()[(drawableArea.y + y) * canvas->width() + drawableArea.x + x].character = {' ', 0, 0, 0};
}

void Painter::fill(Rect rect, const Pixel& pixel) {
    if (rect.x < 0)
        rect.x = 0;
    if (rect.y < 0)
        rect.y = 0;

    if (rect.x + widget->geometry().x < 0)
        rect.x = -widget->geometry().x;
    if (rect.y + widget->geometry().y < 0)
        rect.y = -widget->geometry().y;

    if (rect.x + drawableArea.x < 0)
        rect.x = -drawableArea.x;
    if (rect.y + drawableArea.y < 0)
        rect.y = -drawableArea.y;

    if (rect.x + rect.width > drawableArea.width)
        rect.width = drawableArea.width - rect.x;
    if (rect.y + rect.height > drawableArea.height)
        rect.height = drawableArea.height - rect.y;

    if (rect.x + drawableArea.x >= canvas->width() || rect.y + drawableArea.y >= canvas->height())
        return;

    if (rect.x + rect.width + drawableArea.x >= canvas->width())
        rect.width = canvas->width() - drawableArea.x - rect.x;
    if (rect.y + rect.height + drawableArea.y >= canvas->height())
        rect.height = canvas->height() - drawableArea.y - rect.y;

    if (rect.width <= 0 || rect.height <= 0)
        return;

    // Pixel* pixelArray = canvas->pixelArray();
    // for (int y = rect.y; y < rect.y + rect.height; ++y)
    //     for (int x = rect.x; x < rect.x + rect.width; ++x)
    //         pixelArray[(drawableArea.y + y) * canvas->width() + drawableArea.x + x] = pixel;
    for (int y = rect.y; y < rect.y + rect.height; ++y)
        for (int x = rect.x; x < rect.x + rect.width; ++x)
            setPixel(x, y, pixel);

    // 其他窗口左侧全角处理
    if (rect.x == 0 && (drawableArea.x + rect.x != 0))
        for (int y = rect.y; y < rect.y + rect.height; ++y) {
            Pixel& leftPixel = canvas->pixelArray()[(drawableArea.y + y) * canvas->width() + drawableArea.x + rect.x - 1];
            if (getPrintWidth(leftPixel.character) == 2)
                leftPixel.character = {' ', 0, 0, 0};
        }

    // 其他窗口右侧全角处理
    if (rect.x + rect.width == drawableArea.width && (drawableArea.x + rect.x + rect.width != canvas->width()))
        for (int y = rect.y; y < rect.y + rect.height; ++y) {
            Pixel& rightPixel = canvas->pixelArray()[(drawableArea.y + y) * canvas->width() + drawableArea.x + rect.x + rect.width];
            if (rightPixel.character[0] == 0)
                rightPixel.character = {' ', 0, 0, 0};
        }
}

void Painter::fill(Rect rect, const Color& bgColor) {
    if (rect.x < 0)
        rect.x = 0;
    if (rect.y < 0)
        rect.y = 0;

    if (rect.x + widget->geometry().x < 0)
        rect.x = -widget->geometry().x;
    if (rect.y + widget->geometry().y < 0)
        rect.y = -widget->geometry().y;

    if (rect.x + drawableArea.x < 0)
        rect.x = -drawableArea.x;
    if (rect.y + drawableArea.y < 0)
        rect.y = -drawableArea.y;

    if (rect.x + rect.width > drawableArea.width)
        rect.width = drawableArea.width - rect.x;
    if (rect.y + rect.height > drawableArea.height)
        rect.height = drawableArea.height - rect.y;

    if (rect.x + drawableArea.x >= canvas->width() || rect.y + drawableArea.y >= canvas->height())
        return;

    if (rect.x + rect.width + drawableArea.x >= canvas->width())
        rect.width = canvas->width() - drawableArea.x - rect.x;
    if (rect.y + rect.height + drawableArea.y >= canvas->height())
        rect.height = canvas->height() - drawableArea.y - rect.y;

    if (rect.width <= 0 || rect.height <= 0)
        return;

    // Pixel* pixelArray = canvas->pixelArray();
    // for (int y = rect.y; y < rect.y + rect.height; ++y)
    //     for (int x = rect.x; x < rect.x + rect.width; ++x)
    //         pixelArray[(drawableArea.y + y) * canvas->width() + drawableArea.x + x] = Pixel{Color(), bgColor, {' ', 0, 0, 0}};
    for (int y = rect.y; y < rect.y + rect.height; ++y)
        for (int x = rect.x; x < rect.x + rect.width; ++x)
            setPixel(x, y, Pixel{Color(), bgColor, {' ', 0, 0, 0}});

    // 其他窗口左侧全角处理
    if (rect.x == 0 && (drawableArea.x + rect.x != 0))
        for (int y = rect.y; y < rect.y + rect.height; ++y) {
            Pixel& leftPixel = canvas->pixelArray()[(drawableArea.y + y) * canvas->width() + drawableArea.x + rect.x - 1];
            if (getPrintWidth(leftPixel.character) == 2)
                leftPixel.character = {' ', 0, 0, 0};
        }

    // 其他窗口右侧全角处理
    if (rect.x + rect.width == drawableArea.width && (drawableArea.x + rect.x + rect.width != canvas->width()))
        for (int y = rect.y; y < rect.y + rect.height; ++y) {
            Pixel& rightPixel = canvas->pixelArray()[(drawableArea.y + y) * canvas->width() + drawableArea.x + rect.x + rect.width];
            if (rightPixel.character[0] == 0)
                rightPixel.character = {' ', 0, 0, 0};
        }
}

void Painter::drawText(Rect rect, const std::string_view& text, const Color& fgColor, const Color& bgColor) {
    std::vector<Utf8Char> u8Chars = getUtf8Chars(text);
    drawText(rect, std::span<const Utf8Char>(u8Chars.data(), u8Chars.size()), fgColor, bgColor);
}

void Painter::drawText(Rect rect, const std::string_view& text, const Color& fgColor) {
    std::vector<Utf8Char> u8Chars = getUtf8Chars(text);
    drawText(rect, std::span<const Utf8Char>(u8Chars.data(), u8Chars.size()), fgColor);
}

void Painter::drawText(Rect rect, const std::vector<Utf8Char>& u8cArray, const Color& fgColor, const Color& bgColor) {
    drawText(rect, std::span<const Utf8Char>(u8cArray.data(), u8cArray.size()), fgColor, bgColor);
}

void Painter::drawText(Rect rect, const std::vector<Utf8Char>& u8cArray, const Color& fgColor) {
    drawText(rect, std::span<const Utf8Char>(u8cArray.data(), u8cArray.size()), fgColor);
}

void Painter::drawText(Rect rect, const std::span<const Utf8Char>& u8cArray, const Color& fgColor, const Color& bgColor) {
    if (rect.x < 0 || rect.y < 0 || rect.x >= drawableArea.width || rect.y >= drawableArea.height)
        return;

    int curU8cIndex = 0;

    for (int i = 0; i < rect.height; ++i)
        for (int j = 0; j < rect.width; ++j) {
            if (curU8cIndex >= u8cArray.size())
                break;

            Utf8Char u8c = u8cArray[curU8cIndex];
            bool needBanNextPixel = false;
            if (getPrintWidth(u8c) == 2)
                if (j + 1 >= rect.width && i + 1 >= rect.height)
                    break;
                else if (j + 1 >= rect.width) {
                    // 换行
                    ++i;
                    j = 0;
                    needBanNextPixel = true;
                } else
                    needBanNextPixel = true;

            if (u8c[0] == '\n') {
                ++i;
                j = -1;
                ++curU8cIndex;
                continue;
            }

            setPixel(rect.x + j, rect.y + i, Pixel{fgColor, bgColor, u8c});

            if (needBanNextPixel) {
                ++j;
                setPixel(rect.x + j, rect.y + i, Pixel{fgColor, bgColor, {0, 0, 0, 0}});
            }

            ++curU8cIndex;
        }
}

void Painter::drawText(Rect rect, const std::span<const Utf8Char>& u8cArray, const Color& fgColor) {
    if (rect.x < 0 || rect.y < 0 || rect.x >= drawableArea.width || rect.y >= drawableArea.height)
        return;

    Pixel* pixelArray = canvas->pixelArray();
    int curU8cIndex = 0;

    for (int i = 0; i < rect.height; ++i)
        for (int j = 0; j < rect.width; ++j) {
            if (curU8cIndex >= u8cArray.size())
                break;

            Utf8Char u8c = u8cArray[curU8cIndex];
            bool needBanNextPixel = false;
            if (getPrintWidth(u8c) == 2)
                if (j + 1 >= rect.width && i + 1 >= rect.height)
                    break;
                else if (j + 1 >= rect.width) {
                    // 换行
                    ++i;
                    j = 0;
                    needBanNextPixel = true;
                } else
                    needBanNextPixel = true;

            if (u8c[0] == '\n') {
                ++i;
                j = -1;
                ++curU8cIndex;
                continue;
            }

            Color bgColor;

            if (drawableArea.x + rect.x + j < 0 || drawableArea.y + rect.y + i < 0 || drawableArea.x + rect.x + j >= canvas->width() || drawableArea.y + rect.y + i >= canvas->height())
                bgColor = Color();
            else
                bgColor = pixelArray[(drawableArea.y + rect.y + i) * canvas->width() + drawableArea.x + rect.x + j].backgroundColor;

            setPixel(rect.x + j, rect.y + i, Pixel{fgColor, bgColor, u8c});

            if (needBanNextPixel) {
                ++j;

                if (drawableArea.x + rect.x + j < 0 || drawableArea.y + rect.y + i < 0 || drawableArea.x + rect.x + j >= canvas->width() || drawableArea.y + rect.y + i >= canvas->height())
                    bgColor = Color();
                else
                    bgColor = pixelArray[(drawableArea.y + rect.y + i) * canvas->width() + drawableArea.x + rect.x + j].backgroundColor;

                setPixel(rect.x + j, rect.y + i, Pixel{fgColor, bgColor, {0, 0, 0, 0}});
            }

            ++curU8cIndex;
        }
}

void Painter::reverseFgBgColor(short x, short y) {
    Pixel originPixel = canvas->pixelArray()[(drawableArea.y + y) * canvas->width() + drawableArea.x + x];
    setPixel(x, y, Pixel{originPixel.backgroundColor, originPixel.foregroundColor, originPixel.character});

    if (getPrintWidth(originPixel.character) == 2)
        setPixel(x + 1, y, Pixel{originPixel.backgroundColor, originPixel.foregroundColor, {0, 0, 0, 0}});
}

Rect Painter::getRealDrawableArea(Widget* widget) {
    if (widget->getParent() == nullptr)
        return widget->geometry();

    Rect parentRealDrawableArea = getRealDrawableArea(widget->getParent());
    int parentStartX = parentRealDrawableArea.x;
    int parentStartY = parentRealDrawableArea.y;
    int parentEndX = parentRealDrawableArea.x + parentRealDrawableArea.width;
    int parentEndY = parentRealDrawableArea.y + parentRealDrawableArea.height;

    Rect childDrawableArea = widget->getParent()->getChildDrawableArea(widget);
    int startX = std::max<int>(0, childDrawableArea.x);
    int startY = std::max<int>(0, childDrawableArea.y);

    startX += parentStartX;
    startY += parentStartY;

    startX += widget->geometry().x;
    startY += widget->geometry().y;

    int endX = std::min<int>(startX + childDrawableArea.width, parentEndX);
    int endY = std::min<int>(startY + childDrawableArea.height, parentEndY);

    Rect realDrawableArea;
    realDrawableArea.x = startX;
    realDrawableArea.y = startY;

    realDrawableArea.width = std::max<int>(0, endX - startX);
    realDrawableArea.width = std::min<int>(realDrawableArea.width, widget->geometry().width);

    realDrawableArea.height = std::max<int>(0, endY - startY);
    realDrawableArea.height = std::min<int>(realDrawableArea.height, widget->geometry().height);

    return realDrawableArea;
}
