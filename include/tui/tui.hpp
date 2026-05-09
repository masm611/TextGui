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

#include <tuple>
#include <utility>
#include <mutex>
#include <thread>
#include <queue>

#include "canvas.hpp"
#include "widget.hpp"

typedef struct {
    enum class Type {
        Mouse,
        Keyboard
    } type;

    MouseEvent mouseEvent;
    KeyboardEvent keyboardEvent;
} InputEvent;

class Tui {
public:
    Tui();
    ~Tui();
    static Tui* instance();

    void init();
    void deinit();
    void loop();
    void processEvents();

    void quit() { shouldQuit = true; }

    Canvas* getCanvas() const { return canvas; }

    void addRootWidget(Widget* widget);
    void removeRootWidget(Widget* widget);

    short width() const { return screenWidth; }
    short height() const { return screenHeight; }

    void deleteWidgetLater(Widget* widget) { widgetsToDelete.push_back(widget); }

    void focusWidget(Widget* widget);
    bool isFocusedWidget(Widget* widget) const { return focusedWidget == widget; }
    Widget* getFocusedWidget() const { return focusedWidget; }

    bool isCtrlPressed() const { return ctrlPressed; }
    bool isAltPressed() const { return altPressed; }
    bool isShiftPressed() const { return shiftPressed; }

    bool isMouseHoverWidget(Widget* widget) const;

    bool isEventLoopShouldQuit() const { return shouldQuit; }

    int getRadioBtnPrintWidth() const { return radioBtnPrintWidth; }
    int getArrowBtnPrintWidth() const { return arrowBtnPrintWidth; }

private:
    void update();
    void clearScreenBuffer(bool flush = false);

    std::tuple<int, int> getScreenBufferSize();

    void renderCanvas(const Canvas& canvas, short x, short y);

    void terminalUpdate();

    void writeToConsole(const std::string& str);

    int screenWidth;
    int screenHeight;

    short cursorX = 0;
    short cursorY = 0;

    bool screenBufferToggleLocked = false;

    Canvas* canvas = nullptr;
    std::vector<Widget*> rootWidgets;

    void rootWidgetToTop(Widget* widget);
    Widget* findTopWidgetAtPoint(short x, short y, Widget* rootWidget = nullptr) const;
    Rect getWidgetAbsoluteGeometry(Widget* widget);
    void DispatchEvent(Widget* widget, const MouseEvent& event);
    void DispatchEvent(Widget* widget, const KeyboardEvent& event);

    std::vector<Widget*> widgetsToDelete;
    void deleteWidget(Widget* widget);

    Widget* focusedWidget = nullptr;

    Widget* findRootParentWidget(Widget* widget);

    bool shouldQuit = false;

    bool ctrlPressed = false;
    bool altPressed = false;
    bool shiftPressed = false;

    std::mutex inputEventQueueMutex;
    std::queue<InputEvent> inputEventQueue;
    std::thread eventReaderThread;
    void eventReaderWorker();

    void ensureAllRootWidgetsVisible();
    void adjustRootWidgetCenterPosOutOfScreen(const Size& oldScreenSize, const Size& newScreenSize);

    int testStrRealPrintWidth(const std::string& str);

    int radioBtnPrintWidth = 0;
    int arrowBtnPrintWidth = 0;
};
