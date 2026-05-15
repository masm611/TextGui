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

#include "tui.hpp"

#include <stdexcept>
#include <iostream>
#include <chrono>
#include <thread>

#include "keyboardEvent.hpp"

#include "terminalWindow.hpp"

Tui* tuiInstance = nullptr;

bool enableDebugTerminal = true;
TerminalWindow* debugTerminal = nullptr;

Tui::Tui() {
    tuiInstance = this;

    if (enableDebugTerminal) {
        debugTerminal = new TerminalWindow();
        debugTerminal->setGeometry(Rect(30, 0, 50, 20));
        debugTerminal->setTitle("调试终端");
    }
}

Tui::~Tui() {
    deinit();
    tuiInstance = nullptr;

    if (enableDebugTerminal && debugTerminal != nullptr) {
        delete debugTerminal;
        debugTerminal = nullptr;
    }
}

Tui* Tui::instance() {
    return tuiInstance;
}

std::chrono::milliseconds currentTimestamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
}

void Tui::loop() {
    eventReaderThread = std::thread(&Tui::eventReaderWorker, this);

    if (enableDebugTerminal && debugTerminal != nullptr)
        debugTerminal->show();

    while (true) {
        processEvents();

        if (shouldQuit)
            break;
    }

    if (enableDebugTerminal && debugTerminal != nullptr)
        debugTerminal->hide();

    eventReaderThread.join();
}

void Tui::processEvents() {
    static bool last_lMouseDown_record = false;
    static bool last_rMouseDown_record = false;
    static short last_mouseX_record = 0;
    static short last_mouseY_record = 0;
    static short lMouseDownX_record = 0;
    static short lMouseDownY_record = 0;

    static std::chrono::milliseconds lastFrameUpdateTime = currentTimestamp();

    static bool bufferSizeNeedSync = false;
    static int pauseFrameCnt = 0;

    static bool isWindowPreviouslyZoomed = false;
    static int zoomedForceSyncSizeCnt = 0;

    static short unzoomedWidth = screenWidth;
    static short unzoomedHeight = screenHeight;
    static int unzoomedForceSyncSizeCnt = 0;

    static Widget* draggingWidget = nullptr;

    static std::chrono::milliseconds doubleClickShreshold = std::chrono::milliseconds(500);
    static std::chrono::milliseconds lastClickTime = currentTimestamp() - doubleClickShreshold - std::chrono::milliseconds(1);

    {
        processWidgetsToDelete();

        while (true) {
            InputEvent ie;

            {
                std::lock_guard<std::mutex> lock(inputEventQueueMutex);

                if (inputEventQueue.size() == 0)
                    break;

                ie = inputEventQueue.front();
                inputEventQueue.pop();
            }

            if (ie.type == InputEvent::Type::Mouse) {
                const MouseEvent& me = ie.mouseEvent;

                cursorX = me.x;
                cursorY = me.y;
                bool lMouseDown = me.leftButtonPressed;
                bool rMouseDown = me.rightButtonPressed;

                // 取出上次状态到局部变量
                bool last_lMouseDown = last_lMouseDown_record;
                bool last_rMouseDown = last_rMouseDown_record;
                short last_mouseX = last_mouseX_record;
                short last_mouseY = last_mouseY_record;
                short lMouseDownX = lMouseDownX_record;
                short lMouseDownY = lMouseDownY_record;

                // 保存鼠标状态到下一次事件处理
                if (lMouseDown && !last_lMouseDown) {
                    lMouseDownX_record = me.x;
                    lMouseDownY_record = me.y;
                }
                if (!lMouseDown && last_lMouseDown) {
                    lMouseDownX_record = -1;
                    lMouseDownY_record = -1;
                }
                if (last_mouseX_record != cursorX || last_mouseY_record != cursorY) {
                    last_mouseX_record = cursorX;
                    last_mouseY_record = cursorY;
                }
                last_lMouseDown_record = lMouseDown;
                last_rMouseDown_record = rMouseDown;

                // 窗口置顶处理
                if (!last_lMouseDown && lMouseDown) {
                    for (int i = static_cast<int>(rootWidgets.size()) - 1; i >= 0; --i) {
                        Widget* widget = rootWidgets[i];
                        if (widget->geometry().isPointInRect(cursorX, cursorY)) {
                            rootWidgetToTop(widget);
                            break;
                        }
                    }
                }

                // drag事件处理
                if (!lMouseDown && draggingWidget != nullptr) {
                    // drag松开
                    MouseEvent mouseUpEvent;
                    mouseUpEvent.eventType = MouseEventType::LeftButtonUp;
                    mouseUpEvent.x = me.x;
                    mouseUpEvent.y = me.y;
                    mouseUpEvent.leftButtonPressed = lMouseDown;
                    mouseUpEvent.rightButtonPressed = rMouseDown;

                    DispatchEvent(draggingWidget, mouseUpEvent);
                    draggingWidget = nullptr;
                }
                if ((last_mouseX != cursorX || last_mouseY != cursorY) && lMouseDown && last_lMouseDown) {
                    MouseEvent dragEvent;
                    dragEvent.eventType = MouseEventType::Drag;
                    dragEvent.x = me.x;
                    dragEvent.y = me.y;
                    dragEvent.leftButtonPressed = lMouseDown;
                    dragEvent.rightButtonPressed = rMouseDown;
                    dragEvent.drag_lastX = last_mouseX;
                    dragEvent.drag_lastY = last_mouseY;
                    dragEvent.drag_beginX = lMouseDownX;
                    dragEvent.drag_beginY = lMouseDownY;
                    if (draggingWidget == nullptr) {
                        Widget* topWidget = findTopWidgetAtPoint(lMouseDownX, lMouseDownY);
                        draggingWidget = topWidget;
                    }

                    if (draggingWidget != nullptr)
                        DispatchEvent(draggingWidget, dragEvent);
                }

                // // 系统doubleClick处理
                // if (mer.dwEventFlags == DOUBLE_CLICK) {
                //     // doubleClick也算click，派发事件
                //     MouseEvent clickEvent;
                //     clickEvent.eventType = MouseEventType::Click;
                //     clickEvent.x = mer.dwMousePosition.X;
                //     clickEvent.y = mer.dwMousePosition.Y;
                //     clickEvent.leftButtonPressed = lMouseDown;
                //     clickEvent.rightButtonPressed = rMouseDown;

                //     Widget* topWidget = findTopWidgetAtPoint(clickEvent.x, clickEvent.y);
                //     if (topWidget != nullptr)
                //         DispatchEvent(topWidget, clickEvent);

                //     MouseEvent doubleClickEvent;
                //     doubleClickEvent.eventType = MouseEventType::DoubleClick;
                //     doubleClickEvent.x = mer.dwMousePosition.X;
                //     doubleClickEvent.y = mer.dwMousePosition.Y;
                //     doubleClickEvent.leftButtonPressed = lMouseDown;
                //     doubleClickEvent.rightButtonPressed = rMouseDown;

                //     if (topWidget != nullptr)
                //         DispatchEvent(topWidget, doubleClickEvent);

                //     // 强制判定左键弹起
                //     lMouseDown = false;
                //     last_lMouseDown_record = false;
                // }

                // 内置click事件处理
                if ((abs(lMouseDownX - cursorX) < 2 && abs(lMouseDownY - cursorY) < 1) && !lMouseDown && last_lMouseDown) {
                    MouseEvent clickEvent;
                    clickEvent.eventType = MouseEventType::Click;
                    clickEvent.x = me.x;
                    clickEvent.y = me.y;
                    clickEvent.leftButtonPressed = lMouseDown;
                    clickEvent.rightButtonPressed = rMouseDown;

                    Widget* topWidget = findTopWidgetAtPoint(clickEvent.x, clickEvent.y);
                    if (topWidget != nullptr) {
                        DispatchEvent(topWidget, clickEvent);
                        focusWidget(topWidget);
                    }

                    // 双击事件处理
                    if (currentTimestamp() - lastClickTime <= doubleClickShreshold) {
                        MouseEvent doubleClickEvent;
                        doubleClickEvent.eventType = MouseEventType::DoubleClick;
                        doubleClickEvent.x = me.x;
                        doubleClickEvent.y = me.y;
                        doubleClickEvent.leftButtonPressed = lMouseDown;
                        doubleClickEvent.rightButtonPressed = rMouseDown;

                        if (topWidget != nullptr)
                            DispatchEvent(topWidget, doubleClickEvent);

                        lastClickTime = currentTimestamp() - doubleClickShreshold - std::chrono::milliseconds(1);
                    } else {
                        lastClickTime = currentTimestamp();
                    }
                }

                // lMouseDown事件
                if (lMouseDown && !last_lMouseDown) {
                    MouseEvent lMouseDownEvent;
                    lMouseDownEvent.eventType = MouseEventType::LeftButtonDown;
                    lMouseDownEvent.x = me.x;
                    lMouseDownEvent.y = me.y;
                    lMouseDownEvent.leftButtonPressed = lMouseDown;
                    lMouseDownEvent.rightButtonPressed = rMouseDown;

                    Widget* topWidget = findTopWidgetAtPoint(lMouseDownEvent.x, lMouseDownEvent.y);
                    if (topWidget != nullptr)
                        DispatchEvent(topWidget, lMouseDownEvent);
                }

                // lMouseUp事件
                if (!lMouseDown && last_lMouseDown) {
                    MouseEvent lMouseUpEvent;
                    lMouseUpEvent.eventType = MouseEventType::LeftButtonUp;
                    lMouseUpEvent.x = me.x;
                    lMouseUpEvent.y = me.y;
                    lMouseUpEvent.leftButtonPressed = lMouseDown;
                    lMouseUpEvent.rightButtonPressed = rMouseDown;

                    Widget* topWidget = findTopWidgetAtPoint(lMouseUpEvent.x, lMouseUpEvent.y);
                    if (topWidget != nullptr)
                        DispatchEvent(topWidget, lMouseUpEvent);
                }

                // rMouseDown事件
                if (rMouseDown && !last_rMouseDown) {
                    MouseEvent rMouseDownEvent;
                    rMouseDownEvent.eventType = MouseEventType::RightButtonDown;
                    rMouseDownEvent.x = me.x;
                    rMouseDownEvent.y = me.y;
                    rMouseDownEvent.leftButtonPressed = lMouseDown;
                    rMouseDownEvent.rightButtonPressed = rMouseDown;

                    Widget* topWidget = findTopWidgetAtPoint(rMouseDownEvent.x, rMouseDownEvent.y);
                    if (topWidget != nullptr)
                        DispatchEvent(topWidget, rMouseDownEvent);
                }

                // rMouseUp事件
                if (!rMouseDown && last_rMouseDown) {
                    MouseEvent rMouseUpEvent;
                    rMouseUpEvent.eventType = MouseEventType::RightButtonUp;
                    rMouseUpEvent.x = me.x;
                    rMouseUpEvent.y = me.y;
                    rMouseUpEvent.leftButtonPressed = lMouseDown;
                    rMouseUpEvent.rightButtonPressed = rMouseDown;

                    Widget* topWidget = findTopWidgetAtPoint(rMouseUpEvent.x, rMouseUpEvent.y);
                    if (topWidget != nullptr)
                        DispatchEvent(topWidget, rMouseUpEvent);
                }

                // mouseMoved事件
                if (last_mouseX != cursorX || last_mouseY != cursorY) {
                    MouseEvent mouseMovedEvent;
                    mouseMovedEvent.eventType = MouseEventType::Move;
                    mouseMovedEvent.x = me.x;
                    mouseMovedEvent.y = me.y;
                    mouseMovedEvent.leftButtonPressed = lMouseDown;
                    mouseMovedEvent.rightButtonPressed = rMouseDown;

                    Widget* topWidget = findTopWidgetAtPoint(mouseMovedEvent.x, mouseMovedEvent.y);
                    if (topWidget != nullptr)
                        DispatchEvent(topWidget, mouseMovedEvent);
                }

                // 滚轮事件
                if (ie.mouseEvent.eventType == MouseEventType::WheelUp || ie.mouseEvent.eventType == MouseEventType::WheelDown) {
                    MouseEvent wheelEvent;
                    wheelEvent.x = me.x;
                    wheelEvent.y = me.y;
                    wheelEvent.leftButtonPressed = lMouseDown;
                    wheelEvent.rightButtonPressed = rMouseDown;

                    if (ie.mouseEvent.eventType == MouseEventType::WheelUp)
                        wheelEvent.eventType = MouseEventType::WheelUp;
                    else
                        wheelEvent.eventType = MouseEventType::WheelDown;

                    Widget* topWidget = findTopWidgetAtPoint(wheelEvent.x, wheelEvent.y);
                    if (topWidget != nullptr)
                        DispatchEvent(topWidget, wheelEvent);
                }

                // mouseLeave事件
                {
                    Widget* lastTopWidget = findTopWidgetAtPoint(last_mouseX, last_mouseY);
                    Widget* currentTopWidget = findTopWidgetAtPoint(cursorX, cursorY);

                    if (lastTopWidget != currentTopWidget) {
                        if (lastTopWidget != nullptr) {
                            MouseEvent leaveEvent;
                            leaveEvent.eventType = MouseEventType::Leave;
                            leaveEvent.x = cursorX;
                            leaveEvent.y = cursorY;
                            leaveEvent.leftButtonPressed = lMouseDown;
                            leaveEvent.rightButtonPressed = rMouseDown;

                            DispatchEvent(lastTopWidget, leaveEvent);
                        }
                    }
                }

                // all事件
                {
                    MouseEvent allMouseEvent;
                    allMouseEvent.eventType = MouseEventType::All;
                    allMouseEvent.x = me.x;
                    allMouseEvent.y = me.y;
                    allMouseEvent.leftButtonPressed = lMouseDown;
                    allMouseEvent.rightButtonPressed = rMouseDown;

                    Widget* topWidget = findTopWidgetAtPoint(allMouseEvent.x, allMouseEvent.y);
                    if (topWidget != nullptr)
                        DispatchEvent(topWidget, allMouseEvent);
                }

            } else if (ie.type == InputEvent::Type::Keyboard) {
                KeyboardEvent& ke = ie.keyboardEvent;

                ctrlPressed = ke.ctrlPressed;
                altPressed = ke.altPressed;
                shiftPressed = ke.shiftPressed;
                if (ctrlPressed && (ke.keyChar == 'c' || ke.keyChar == 'C')) {
                    std::cout << "Ctrl+C 被按下，退出程序。\n";
                    shouldQuit = true;
                }

                // 派发键盘事件
                if (focusedWidget != nullptr) {
                    if (ke.keyChar == '\r')
                        ke.keyChar = '\n';

                    // keyDown/KeyUp事件
                    KeyboardEvent keyboardEvent;
                    keyboardEvent.ctrlPressed = ctrlPressed;
                    keyboardEvent.altPressed = altPressed;
                    keyboardEvent.shiftPressed = shiftPressed;
                    keyboardEvent.keyChar = ke.keyChar;
                    keyboardEvent.virtualKeyCode = ke.virtualKeyCode;
                    keyboardEvent.eventType = ke.eventType;

                    DispatchEvent(focusedWidget, keyboardEvent);

                    if (ke.eventType == KeyboardEventType::KeyDown) {
                        if (enableDebugTerminal)
                            debugTerminal->write(std::format("KeyDown: char={}, vkCode={}\n", (int)ke.keyChar, ke.virtualKeyCode));

                        if ((ke.keyChar >= 32 && ke.keyChar != 127) || ke.keyChar < 0 || ke.keyChar == '\n' || ke.keyChar == '\b') {
                            // CharInput事件
                            KeyboardEvent charInputEvent;
                            charInputEvent.ctrlPressed = ctrlPressed;
                            charInputEvent.altPressed = altPressed;
                            charInputEvent.shiftPressed = shiftPressed;
                            charInputEvent.keyChar = ke.keyChar;
                            charInputEvent.virtualKeyCode = ke.virtualKeyCode;
                            charInputEvent.eventType = KeyboardEventType::CharInput;

                            DispatchEvent(focusedWidget, charInputEvent);
                        } else {
                            // SpecialInput事件
                            KeyboardEvent specialInputEvent;
                            specialInputEvent.ctrlPressed = ctrlPressed;
                            specialInputEvent.altPressed = altPressed;
                            specialInputEvent.shiftPressed = shiftPressed;
                            specialInputEvent.keyChar = ke.keyChar;
                            specialInputEvent.virtualKeyCode = ke.virtualKeyCode;
                            specialInputEvent.eventType = KeyboardEventType::SpecialInput;

                            DispatchEvent(focusedWidget, specialInputEvent);
                        }
                    }
                }
            }
        }

        terminalUpdate();

        ensureAllRootWidgetsVisible();

        update();

        auto sleepTime = std::chrono::milliseconds(16) - (currentTimestamp() - lastFrameUpdateTime);
        lastFrameUpdateTime = currentTimestamp();
        if (sleepTime.count() > 0) {
            std::this_thread::sleep_for(sleepTime);
        }
    }
}

void Tui::addRootWidget(Widget* widget) {
    rootWidgets.push_back(widget);
}

void Tui::removeRootWidget(Widget* widget) {
    for (int i = 0; i < rootWidgets.size(); ++i) {
        if (rootWidgets[i] == widget) {
            rootWidgets.erase(rootWidgets.begin() + i);
            break;
        }
    }
}

int frameCnt = 0;
int frameCnt_1s = 0;
std::chrono::milliseconds lastFpsUpdateFpsTime = currentTimestamp();
double fps = 0.0;

void Tui::focusWidget(Widget* widget) {
    if (focusedWidget == widget)
        return;

    focusedWidget = widget;

    if (widget == nullptr)
        return;

    findRootParentWidget(widget);
    rootWidgetToTop(focusedWidget);
}

bool Tui::isMouseHoverWidget(Widget* widget) const {
    Widget* topWidget = findTopWidgetAtPoint(cursorX, cursorY, nullptr);
    return topWidget == widget;
}

void Tui::update() {
    this->canvas = new Canvas(screenWidth, screenHeight);

    ++frameCnt;
    ++frameCnt_1s;
    if (currentTimestamp() - lastFpsUpdateFpsTime >= std::chrono::milliseconds(1000)) {
        fps = frameCnt_1s * 1000.0 / (currentTimestamp() - lastFpsUpdateFpsTime).count();
        lastFpsUpdateFpsTime = currentTimestamp();
        frameCnt_1s = 0;
    }

    clearScreenBuffer(false);

    canvas->drawText(Rect(0, 0, 50, 1), std::format("frameCnt: {}, FPS: {:.2f}", frameCnt, fps), Color(255, 255, 255));
    canvas->drawText(Rect(0, 1, 50, 1), std::format("Cursor: ({}, {})", cursorX, cursorY), Color(255, 255, 255));
    canvas->drawText(Rect(0, 2, 50, 1), std::format("{}x{}", screenWidth, screenHeight), Color(255, 255, 255));

    for (int i = 0; i <= 26; ++i) {
        int brightness = i * 10;
        if (brightness > 255) brightness = 255;
        canvas->fill(Rect(i, 3, 1, 1), Color(brightness, 0, 0));
    }
    for (int i = 0; i <= 26; ++i) {
        int brightness = i * 10;
        if (brightness > 255) brightness = 255;
        canvas->fill(Rect(i, 4, 1, 1), Color(0, brightness, 0));
    }
    for (int i = 0; i <= 26; ++i) {
        int brightness = i * 10;
        if (brightness > 255) brightness = 255;
        canvas->fill(Rect(i, 5, 1, 1), Color(0, 0, brightness));
    }

    for (int i = 0; i < rootWidgets.size(); ++i) {
        Widget* widget = rootWidgets[i];
        if (widget->isVisible())
            widget->draw();
    }

    renderCanvas(*this->canvas, 0, 0);

    delete this->canvas;
}

void Tui::renderCanvas(const Canvas& canvas, short x, short y) {
    std::string vtStr;

    vtStr += std::format("\x1b[{};{}H", y + 1, x + 1);

    Color lastForegroundColor(0, 0, 0);
    Color lastBackgroundColor(0, 0, 0);
    vtStr += std::format("\x1b[48;2;{};{};{};38;2;{};{};{}m", lastBackgroundColor.r(), lastBackgroundColor.g(), lastBackgroundColor.b(), lastForegroundColor.r(), lastForegroundColor.g(), lastForegroundColor.b());

    auto [width, height] = canvas.getSize();

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const Pixel& pix = canvas.getPixel(j, i);

            if (pix.character[0] == 0)
                continue;

            if (!(pix.backgroundColor == lastBackgroundColor && pix.foregroundColor == lastForegroundColor)) {
                vtStr += std::format("\x1b[48;2;{};{};{};38;2;{};{};{}m", pix.backgroundColor.r(), pix.backgroundColor.g(), pix.backgroundColor.b(), pix.foregroundColor.r(), pix.foregroundColor.g(), pix.foregroundColor.b());
                lastBackgroundColor = pix.backgroundColor;
                lastForegroundColor = pix.foregroundColor;
            }

            for (int k = 0; k < 4 && pix.character[k] != 0; ++k)
                vtStr += pix.character[k];
        }

        vtStr += std::format("\x1b[{};{}H", y + i + 2, x + 1);
    }

    writeToConsole(vtStr);
}

void Tui::rootWidgetToTop(Widget* widget) {
    for (int i = 0; i < rootWidgets.size(); ++i) {
        if (rootWidgets[i] == widget) {
            rootWidgets.erase(rootWidgets.begin() + i);
            rootWidgets.push_back(widget);
            break;
        }
    }
}

Widget* Tui::findTopWidgetAtPoint(short x, short y, Widget* rootWidget) const {
    if (rootWidget == nullptr) {
        for (int i = static_cast<int>(rootWidgets.size()) - 1; i >= 0; --i) {
            Widget* widget = rootWidgets[i];
            if (widget->geometry().isPointInRect(x, y)) {
                Widget* childResult = findTopWidgetAtPoint(x - widget->geometry().x, y - widget->geometry().y, widget);
                if (childResult != nullptr)
                    return childResult;
                else
                    return widget;
            }
        }
    } else {
        for (int i = static_cast<int>(rootWidget->getChildren().size()) - 1; i >= 0; --i) {
            Widget* widget = rootWidget->getChildren()[i];
            if (widget->geometry().isPointInRect(x - rootWidget->getChildDrawableArea(widget).x, y - rootWidget->getChildDrawableArea(widget).y)) {
                Widget* childResult = findTopWidgetAtPoint(x - widget->geometry().x - rootWidget->getChildDrawableArea(widget).x, y - widget->geometry().y - rootWidget->getChildDrawableArea(widget).y, widget);
                if (childResult != nullptr)
                    return childResult;
                else
                    return widget;
            }
        }
    }

    return nullptr;
}

Rect Tui::getWidgetAbsoluteGeometry(Widget* widget) {
    if (widget->getParent() == nullptr) {
        return widget->geometry();
    } else {
        Rect parentRect = getWidgetAbsoluteGeometry(widget->getParent());
        Rect widgetRect = widget->geometry();
        Rect childDrawableArea = widget->getParent()->getChildDrawableArea(widget);
        return Rect(parentRect.x + widgetRect.x + childDrawableArea.x, parentRect.y + widgetRect.y + childDrawableArea.y, widgetRect.width, widgetRect.height);
    }
}

void Tui::DispatchEvent(Widget* widget, const MouseEvent& event) {
    if (widget == nullptr)
        return;

    MouseEvent transformedEvent = event;
    Rect absWidgetRect = getWidgetAbsoluteGeometry(widget);
    transformedEvent.x -= absWidgetRect.x;
    transformedEvent.y -= absWidgetRect.y;
    transformedEvent.drag_lastX -= absWidgetRect.x;
    transformedEvent.drag_lastY -= absWidgetRect.y;
    transformedEvent.drag_beginX -= absWidgetRect.x;
    transformedEvent.drag_beginY -= absWidgetRect.y;
    EventProcessResult res = widget->mouseEvent(transformedEvent);

    if (res == EventProcessResult::PassDown) {
        Widget* parent = widget->getParent();
        if (parent != nullptr) {
            DispatchEvent(parent, event);
        }
    }
}

void Tui::DispatchEvent(Widget* widget, const KeyboardEvent& event) {
    if (widget == nullptr)
        return;

    EventProcessResult res = widget->keyboardEvent(event);

    if (res == EventProcessResult::PassDown) {
        Widget* parent = widget->getParent();
        if (parent != nullptr) {
            DispatchEvent(parent, event);
        }
    }
}

void Tui::processWidgetsToDelete() {
    for (Widget* widget : widgetsToDelete) {
        delete widget;
    }
    widgetsToDelete.clear();
}

Widget* Tui::findRootParentWidget(Widget* widget) {
    if (widget->getParent() == nullptr) {
        return widget;
    } else {
        return findRootParentWidget(widget->getParent());
    }
}

void Tui::ensureAllRootWidgetsVisible() {
    for (Widget* widget : rootWidgets) {
        Rect geom = widget->geometry();

        int newX = geom.x;
        int newY = geom.y;

        if (geom.x >= screenWidth)
            newX = screenWidth - 1;
        if (geom.y >= screenHeight)
            newY = screenHeight - 1;

        if (newY < 0)
            newY = 0;

        if (newX != geom.x || newY != geom.y)
            widget->setGeometry(Rect(newX, newY, geom.width, geom.height));
    }
}

void Tui::adjustRootWidgetCenterPosOutOfScreen(const Size& oldScreenSize, const Size& newScreenSize) {
    float widthRatio = (float)newScreenSize.width / oldScreenSize.width;
    float heightRatio = (float)newScreenSize.height / oldScreenSize.height;

    for (Widget* widget : rootWidgets) {
        Rect geom = widget->geometry();

        int windowCenterX = geom.x + geom.width / 2;
        int windowCenterY = geom.y + geom.height / 2;

        if (windowCenterX < newScreenSize.width && windowCenterY < newScreenSize.height)
            continue;

        int newWindowCenterX = (int)(windowCenterX * widthRatio);
        int newWindowCenterY = (int)(windowCenterY * heightRatio);

        int newX = newWindowCenterX - geom.width / 2;
        int newY = newWindowCenterY - geom.height / 2;

        if (newX != geom.x || newY != geom.y)
            widget->setGeometry(Rect(newX, newY, geom.width, geom.height));
    }
}
