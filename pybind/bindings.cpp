#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>

#include "tui/tui.hpp"
#include "tui/widget.hpp"
#include "tui/window.hpp"
#include "tui/button.hpp"
#include "tui/label.hpp"
#include "tui/lineTextInput.hpp"
#include "tui/terminalWindow.hpp"
#include "tui/radioButton.hpp"
#include "tui/radioButtonGroup.hpp"
#include "tui/scrollWidget.hpp"
#include "tui/tableWidget.hpp"
#include "tui/canvasWidget.hpp"
#include "tui/horizontalLayoutWidget.hpp"
#include "tui/verticalLayoutWidget.hpp"
#include "tui/marginContainerWidget.hpp"
#include "tui/maximizeContainerWidget.hpp"
#include "tui/windowResizeHandle.hpp"
#include "tui/canvas.hpp"
#include "tui/painter.hpp"
#include "tui/messagebox.hpp"
#include "tui/utf8Utils.hpp"
#include "tui/color.hpp"
#include "tui/rect.hpp"
#include "tui/size.hpp"
#include "tui/pixel.hpp"
#include "tui/sizePolicy.hpp"
#include "tui/eventProcessResult.hpp"
#include "tui/mouseEvent.hpp"
#include "tui/keyboardEvent.hpp"

namespace py = pybind11;

// ============================================================================
// Utf8Char <-> Python str 自定义类型转换器
// ============================================================================
namespace pybind11 {
namespace detail {
template <>
struct type_caster<Utf8Char> {
public:
    PYBIND11_TYPE_CASTER(Utf8Char, const_name("str"));

    // Python str -> Utf8Char
    bool load(handle src, bool) {
        if (!py::isinstance<py::str>(src))
            return false;
        std::string s = py::cast<std::string>(src);
        auto chars = getUtf8Chars(s);
        if (chars.empty()) {
            value = Utf8Char{'\0', 0, 0, 0};
        } else {
            value = chars[0];
        }
        return true;
    }

    // Utf8Char -> Python str
    static handle cast(const Utf8Char& src, return_value_policy /* policy */, handle /* parent */) {
        std::vector<Utf8Char> vec{src};
        std::string s = u8cArrayToString(vec);
        return py::str(s).release();
    }
};
}  // namespace detail
}  // namespace pybind11

// ============================================================================
// Widget Trampoline Class
// ============================================================================
class PyWidget : public Widget {
public:
    using Widget::Widget;

    void draw() override {
        PYBIND11_OVERRIDE(void, Widget, draw);
    }

    Rect geometry() const override {
        PYBIND11_OVERRIDE(Rect, Widget, geometry);
    }

    void setGeometry(const Rect& newGeometry) override {
        PYBIND11_OVERRIDE(void, Widget, setGeometry, newGeometry);
    }

    Rect getChildDrawableArea(const Widget* childWidget) const override {
        PYBIND11_OVERRIDE(Rect, Widget, getChildDrawableArea, childWidget);
    }

    EventProcessResult mouseEvent(const MouseEvent& event) override {
        PYBIND11_OVERRIDE(EventProcessResult, Widget, mouseEvent, event);
    }

    EventProcessResult keyboardEvent(const KeyboardEvent& event) override {
        PYBIND11_OVERRIDE(EventProcessResult, Widget, keyboardEvent, event);
    }

    void hide() override {
        PYBIND11_OVERRIDE(void, Widget, hide);
    }

    SizePolicy getVerticalSizePolicy() const override {
        PYBIND11_OVERRIDE(SizePolicy, Widget, getVerticalSizePolicy);
    }

    SizePolicy getHorizontalSizePolicy() const override {
        PYBIND11_OVERRIDE(SizePolicy, Widget, getHorizontalSizePolicy);
    }

    Size getMinimumSize() const override {
        PYBIND11_OVERRIDE(Size, Widget, getMinimumSize);
    }

    void setMinimumSize(const Size& newMinSize) override {
        PYBIND11_OVERRIDE(void, Widget, setMinimumSize, newMinSize);
    }
};

// ============================================================================
// Window Trampoline Class
// ============================================================================
class PyWindow : public Window {
public:
    using Window::Window;

    void draw() override {
        PYBIND11_OVERRIDE(void, Window, draw);
    }

    Rect getChildDrawableArea(const Widget* childWidget) const override {
        PYBIND11_OVERRIDE(Rect, Window, getChildDrawableArea, childWidget);
    }

    EventProcessResult mouseEvent(const MouseEvent& event) override {
        PYBIND11_OVERRIDE(EventProcessResult, Window, mouseEvent, event);
    }

    Rect geometry() const override {
        PYBIND11_OVERRIDE(Rect, Window, geometry);
    }
};

// ============================================================================
// Button Trampoline Class
// ============================================================================
class PyButton : public Button {
public:
    using Button::Button;

    void draw() override {
        PYBIND11_OVERRIDE(void, Button, draw);
    }

    EventProcessResult mouseEvent(const MouseEvent& event) override {
        PYBIND11_OVERRIDE(EventProcessResult, Button, mouseEvent, event);
    }
};

// ============================================================================
// Label Trampoline Class
// ============================================================================
class PyLabel : public Label {
public:
    using Label::Label;

    void draw() override {
        PYBIND11_OVERRIDE(void, Label, draw);
    }
};

// ============================================================================
// LineTextInput Trampoline Class
// ============================================================================
class PyLineTextInput : public LineTextInput {
public:
    using LineTextInput::LineTextInput;

    void draw() override {
        PYBIND11_OVERRIDE(void, LineTextInput, draw);
    }

    EventProcessResult keyboardEvent(const KeyboardEvent& event) override {
        PYBIND11_OVERRIDE(EventProcessResult, LineTextInput, keyboardEvent, event);
    }

    EventProcessResult mouseEvent(const MouseEvent& event) override {
        PYBIND11_OVERRIDE(EventProcessResult, LineTextInput, mouseEvent, event);
    }
};

// ============================================================================
// RadioButton Trampoline Class
// ============================================================================
class PyRadioButton : public RadioButton {
public:
    using RadioButton::RadioButton;

    void draw() override {
        PYBIND11_OVERRIDE(void, RadioButton, draw);
    }

    EventProcessResult mouseEvent(const MouseEvent& event) override {
        PYBIND11_OVERRIDE(EventProcessResult, RadioButton, mouseEvent, event);
    }
};

// ============================================================================
// ScrollWidget Trampoline Class
// ============================================================================
class PyScrollWidget : public ScrollWidget {
public:
    using ScrollWidget::ScrollWidget;

    void draw() override {
        PYBIND11_OVERRIDE(void, ScrollWidget, draw);
    }

    Rect getChildDrawableArea(const Widget* childWidget) const override {
        PYBIND11_OVERRIDE(Rect, ScrollWidget, getChildDrawableArea, childWidget);
    }

    EventProcessResult mouseEvent(const MouseEvent& event) override {
        PYBIND11_OVERRIDE(EventProcessResult, ScrollWidget, mouseEvent, event);
    }

    EventProcessResult keyboardEvent(const KeyboardEvent& event) override {
        PYBIND11_OVERRIDE(EventProcessResult, ScrollWidget, keyboardEvent, event);
    }
};

// ============================================================================
// TableWidget Trampoline Class
// ============================================================================
class PyTableWidget : public TableWidget {
public:
    using TableWidget::TableWidget;

    void draw() override {
        PYBIND11_OVERRIDE(void, TableWidget, draw);
    }
};

// ============================================================================
// CanvasWidget Trampoline Class
// ============================================================================
class PyCanvasWidget : public CanvasWidget {
public:
    using CanvasWidget::CanvasWidget;

    void draw() override {
        PYBIND11_OVERRIDE(void, CanvasWidget, draw);
    }
};

// ============================================================================
// HorizontalLayoutWidget Trampoline Class
// ============================================================================
class PyHorizontalLayoutWidget : public HorizontalLayoutWidget {
public:
    using HorizontalLayoutWidget::HorizontalLayoutWidget;

    void draw() override {
        PYBIND11_OVERRIDE(void, HorizontalLayoutWidget, draw);
    }
};

// ============================================================================
// VerticalLayoutWidget Trampoline Class
// ============================================================================
class PyVerticalLayoutWidget : public VerticalLayoutWidget {
public:
    using VerticalLayoutWidget::VerticalLayoutWidget;

    void draw() override {
        PYBIND11_OVERRIDE(void, VerticalLayoutWidget, draw);
    }
};

// ============================================================================
// MarginContainerWidget Trampoline Class
// ============================================================================
class PyMarginContainerWidget : public MarginContainerWidget {
public:
    using MarginContainerWidget::MarginContainerWidget;

    void draw() override {
        PYBIND11_OVERRIDE(void, MarginContainerWidget, draw);
    }

    Rect getChildDrawableArea(const Widget* childWidget) const override {
        PYBIND11_OVERRIDE(Rect, MarginContainerWidget, getChildDrawableArea, childWidget);
    }
};

// ============================================================================
// MaximizeContainerWidget Trampoline Class
// ============================================================================
class PyMaximizeContainerWidget : public MaximizeContainerWidget {
public:
    using MaximizeContainerWidget::MaximizeContainerWidget;

    void draw() override {
        PYBIND11_OVERRIDE(void, MaximizeContainerWidget, draw);
    }
};

// ============================================================================
// WindowResizeHandle Trampoline Class
// ============================================================================
class PyWindowResizeHandle : public WindowResizeHandle {
public:
    using WindowResizeHandle::WindowResizeHandle;

    void draw() override {
        PYBIND11_OVERRIDE(void, WindowResizeHandle, draw);
    }

    EventProcessResult mouseEvent(const MouseEvent& event) override {
        PYBIND11_OVERRIDE(EventProcessResult, WindowResizeHandle, mouseEvent, event);
    }
};

// ============================================================================
// TerminalWindow Trampoline Class
// ============================================================================
class PyTerminalWindow : public TerminalWindow {
public:
    using TerminalWindow::TerminalWindow;

    void draw() override {
        PYBIND11_OVERRIDE(void, TerminalWindow, draw);
    }
};

PYBIND11_MODULE(textgui, m) {
    m.doc() = "Python bindings for TextGui TUI library";

    // ========================================================================
    // 枚举类型绑定
    // ========================================================================

    py::enum_<MouseEventType>(m, "MouseEventType")
        .value("Move", MouseEventType::Move)
        .value("Click", MouseEventType::Click)
        .value("DoubleClick", MouseEventType::DoubleClick)
        .value("Drag", MouseEventType::Drag)
        .value("LeftButtonDown", MouseEventType::LeftButtonDown)
        .value("LeftButtonUp", MouseEventType::LeftButtonUp)
        .value("RightButtonDown", MouseEventType::RightButtonDown)
        .value("RightButtonUp", MouseEventType::RightButtonUp)
        .value("WheelUp", MouseEventType::WheelUp)
        .value("WheelDown", MouseEventType::WheelDown)
        .value("Leave", MouseEventType::Leave)
        .value("All", MouseEventType::All)
        .export_values();

    py::enum_<KeyboardEventType>(m, "KeyboardEventType")
        .value("KeyDown", KeyboardEventType::KeyDown)
        .value("KeyUp", KeyboardEventType::KeyUp)
        .value("CharInput", KeyboardEventType::CharInput)
        .value("SpecialInput", KeyboardEventType::SpecialInput)
        .value("All", KeyboardEventType::All)
        .export_values();

    py::enum_<EventProcessResult>(m, "EventProcessResult")
        .value("PassDown", EventProcessResult::PassDown)
        .value("Finished", EventProcessResult::Finished)
        .export_values();

    py::enum_<SizePolicy>(m, "SizePolicy")
        .value("Fixed", SizePolicy::Fixed)
        .value("Expanding", SizePolicy::Expanding)
        .value("Shrink", SizePolicy::Shrink)
        .export_values();

    py::enum_<InputEvent::Type>(m, "InputEventType")
        .value("Mouse", InputEvent::Type::Mouse)
        .value("Keyboard", InputEvent::Type::Keyboard)
        .export_values();

    py::enum_<SpecialVirtualKeyCode::Code>(m, "SpecialVirtualKeyCode")
        .value("LeftArrow", SpecialVirtualKeyCode::Code::LeftArrow)
        .value("UpArrow", SpecialVirtualKeyCode::Code::UpArrow)
        .value("RightArrow", SpecialVirtualKeyCode::Code::RightArrow)
        .value("DownArrow", SpecialVirtualKeyCode::Code::DownArrow)
        .export_values();

    // ========================================================================
    // 数据结构绑定
    // ========================================================================

    py::class_<Color>(m, "Color",
                      "RGB color representation")
        .def(py::init<>(), "Default constructor (black)")
        .def(py::init<uint8_t, uint8_t, uint8_t>(),
             py::arg("r"), py::arg("g"), py::arg("b"),
             "Create color with RGB values")
        .def(py::init<const Color&>(), "Copy constructor")
        .def("r", &Color::r, "Get red component")
        .def("g", &Color::g, "Get green component")
        .def("b", &Color::b, "Get blue component")
        .def("setR", &Color::setR, py::arg("r"), "Set red component")
        .def("setG", &Color::setG, py::arg("g"), "Set green component")
        .def("setB", &Color::setB, py::arg("b"), "Set blue component")
        .def("setRGB", &Color::setRGB, py::arg("r"), py::arg("g"), py::arg("b"),
             "Set all RGB components")
        .def("__getitem__", &Color::operator[], py::arg("index"),
             "Get color component by index (0=R, 1=G, 2=B)")
        .def("__eq__", &Color::operator==, py::arg("other"),
             "Check equality")
        .def("__repr__", [](const Color& c) {
            return "Color(" + std::to_string(c.r()) + ", " +
                   std::to_string(c.g()) + ", " + std::to_string(c.b()) + ")";
        });

    py::class_<Rect>(m, "Rect",
                     "Rectangle with position (x, y) and size (width, height)")
        .def(py::init<>())
        .def(py::init([](short x, short y, short width, short height) {
                 return Rect{x, y, width, height};
             }),
             py::arg("x"), py::arg("y"), py::arg("width"), py::arg("height"),
             "Create rect with x, y, width, height")
        .def_readwrite("x", &Rect::x)
        .def_readwrite("y", &Rect::y)
        .def_readwrite("width", &Rect::width)
        .def_readwrite("height", &Rect::height)
        .def("isPointInRect", &Rect::isPointInRect, py::arg("px"), py::arg("py"),
             "Check if a point is inside this rectangle")
        .def("__eq__", &Rect::operator==, py::arg("other"),
             "Check equality")
        .def("__repr__", [](const Rect& r) {
            return "Rect(" + std::to_string(r.x) + ", " + std::to_string(r.y) +
                   ", " + std::to_string(r.width) + ", " + std::to_string(r.height) + ")";
        });

    py::class_<Size>(m, "Size",
                     "Size with width and height")
        .def(py::init<>())
        .def(py::init<int, int>(), py::arg("width"), py::arg("height"))
        .def_readwrite("width", &Size::width)
        .def_readwrite("height", &Size::height)
        .def("__repr__", [](const Size& s) {
            return "Size(" + std::to_string(s.width) + ", " + std::to_string(s.height) + ")";
        });

    py::class_<Pixel>(m, "Pixel",
                      "A single pixel with foreground/background colors and a character")
        .def(py::init<>())
        .def(py::init<const Color&, const Color&, const Utf8Char&>(),
             py::arg("foregroundColor"), py::arg("backgroundColor"), py::arg("character"))
        .def_readwrite("foregroundColor", &Pixel::foregroundColor)
        .def_readwrite("backgroundColor", &Pixel::backgroundColor)
        .def_readwrite("character", &Pixel::character)
        .def("__repr__", [](const Pixel& p) {
            std::vector<Utf8Char> vec{p.character};
            std::string ch = u8cArrayToString(vec);
            return "Pixel(char='" + ch + "')";
        });

    py::class_<MouseEvent>(m, "MouseEvent",
                           "Mouse event data")
        .def(py::init<>())
        .def_readwrite("x", &MouseEvent::x)
        .def_readwrite("y", &MouseEvent::y)
        .def_readwrite("leftButtonPressed", &MouseEvent::leftButtonPressed)
        .def_readwrite("rightButtonPressed", &MouseEvent::rightButtonPressed)
        .def_readwrite("eventType", &MouseEvent::eventType)
        .def_readwrite("drag_lastX", &MouseEvent::drag_lastX)
        .def_readwrite("drag_lastY", &MouseEvent::drag_lastY)
        .def_readwrite("drag_beginX", &MouseEvent::drag_beginX)
        .def_readwrite("drag_beginY", &MouseEvent::drag_beginY);

    py::class_<KeyboardEvent>(m, "KeyboardEvent",
                              "Keyboard event data")
        .def(py::init<>())
        .def_readwrite("ctrlPressed", &KeyboardEvent::ctrlPressed)
        .def_readwrite("shiftPressed", &KeyboardEvent::shiftPressed)
        .def_readwrite("altPressed", &KeyboardEvent::altPressed)
        .def_readwrite("pressedKeys", &KeyboardEvent::pressedKeys)
        .def_readwrite("keyChar", &KeyboardEvent::keyChar)
        .def_readwrite("virtualKeyCode", &KeyboardEvent::virtualKeyCode)
        .def_readwrite("eventType", &KeyboardEvent::eventType);

    // ========================================================================
    // Widget 基类绑定 (含 trampoline class)
    // ========================================================================

    py::class_<Widget, PyWidget>(m, "Widget",
                                 "Base class for all UI widgets")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create a widget with optional parent")
        .def("draw", &Widget::draw,
             "Draw the widget (override in Python subclass for custom drawing)")
        .def("geometry", &Widget::geometry,
             "Get the widget's geometry rectangle")
        .def("setGeometry", &Widget::setGeometry, py::arg("newGeometry"),
             "Set the widget's geometry rectangle")
        .def("getParent", &Widget::getParent,
             "Get parent widget", py::return_value_policy::reference)
        .def("setParent", &Widget::setParent, py::arg("newParent"),
             "Set parent widget")
        .def("getChildren", &Widget::getChildren,
             "Get list of child widgets", py::return_value_policy::copy)
        .def("addChild", &Widget::addChild, py::arg("child"),
             "Add a child widget")
        .def("removeChild", &Widget::removeChild, py::arg("child"),
             "Remove a child widget")
        .def("getChildDrawableArea", &Widget::getChildDrawableArea, py::arg("childWidget"),
             "Get drawable area for a child widget")
        .def("setBackgroundColor", &Widget::setBackgroundColor, py::arg("color"),
             "Set background color")
        .def("disableBackgroundColor", &Widget::disableBackgroundColor,
             "Disable custom background color")
        .def("mouseEvent", &Widget::mouseEvent, py::arg("event"),
             "Handle mouse event (override in Python subclass)")
        .def("keyboardEvent", &Widget::keyboardEvent, py::arg("event"),
             "Handle keyboard event (override in Python subclass)")
        .def("deleteLater", &Widget::deleteLater,
             "Schedule widget for deletion")
        .def("show", &Widget::show,
             "Show the widget")
        .def("hide", &Widget::hide,
             "Hide the widget")
        .def("isVisible", &Widget::isVisible,
             "Check if widget is visible")
        .def("getVerticalSizePolicy", &Widget::getVerticalSizePolicy,
             "Get vertical size policy")
        .def("getHorizontalSizePolicy", &Widget::getHorizontalSizePolicy,
             "Get horizontal size policy")
        .def("setVerticalSizePolicy", &Widget::setVerticalSizePolicy, py::arg("policy"),
             "Set vertical size policy")
        .def("setHorizontalSizePolicy", &Widget::setHorizontalSizePolicy, py::arg("policy"),
             "Set horizontal size policy")
        .def("getMinimumSize", &Widget::getMinimumSize,
             "Get minimum size")
        .def("setMinimumSize", &Widget::setMinimumSize, py::arg("newMinSize"),
             "Set minimum size")
        // 事件回调专用 Python API
        .def("on_mouse_event", [](Widget& self, std::function<void(const MouseEvent&)> callback) { return self.addEventListener("mouse", std::any(callback)); }, py::arg("callback"), "Register mouse event callback: callback(MouseEvent)")
        .def("on_mouse_clicked", [](Widget& self, std::function<void(const MouseEvent&)> callback) { return self.addEventListener("mouseClicked", std::any(callback)); }, py::arg("callback"), "Register mouse clicked event callback: callback(MouseEvent)")
        .def("on_mouse_moved", [](Widget& self, std::function<void(const MouseEvent&)> callback) { return self.addEventListener("mouseMoved", std::any(callback)); }, py::arg("callback"), "Register mouse moved event callback: callback(MouseEvent)")
        .def("on_mouse_drag", [](Widget& self, std::function<void(const MouseEvent&)> callback) { return self.addEventListener("mouseDrag", std::any(callback)); }, py::arg("callback"), "Register mouse drag event callback: callback(MouseEvent)")
        .def("on_keyboard_event", [](Widget& self, std::function<void(const KeyboardEvent&)> callback) { return self.addEventListener("keyboard", std::any(callback)); }, py::arg("callback"), "Register keyboard event callback: callback(KeyboardEvent)")
        .def("removeEventListener", &Widget::removeEventListener, py::arg("eventName"), py::arg("listenerId"), "Remove an event listener by name and ID");

    // ========================================================================
    // Window 类绑定
    // ========================================================================

    py::class_<Window, Widget, PyWindow>(m, "Window",
                                         "Window widget with title bar and close button")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr)
        .def("draw", &Window::draw)
        .def("getChildDrawableArea", &Window::getChildDrawableArea, py::arg("childWidget"))
        .def("mouseEvent", &Window::mouseEvent, py::arg("event"))
        .def("geometry", &Window::geometry)
        .def("setTitleColor", &Window::setTitleColor, py::arg("fgColor"), py::arg("bgColor"),
             "Set title bar colors")
        .def("setTitle", &Window::setTitle, py::arg("title"),
             "Set window title")
        .def("maximizeWindow", &Window::maximizeWindow,
             "Maximize the window")
        .def("unmaximizeWindow", &Window::unmaximizeWindow,
             "Restore the window from maximized state")
        // 事件回调
        .def("on_close", [](Window& self, std::function<void()> callback) { return self.addEventListener("windowClose", std::any(callback)); }, py::arg("callback"), "Register window close callback: callback()");

    // ========================================================================
    // Button 类绑定
    // ========================================================================

    py::class_<Button, Widget, PyButton>(m, "Button",
                                         "Clickable button widget")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create an empty button")
        .def(py::init<const std::string&, Widget*>(), py::arg("text"), py::arg("parent") = nullptr,
             "Create a button with text")
        .def("draw", &Button::draw)
        .def("mouseEvent", &Button::mouseEvent, py::arg("event"))
        .def("setText", &Button::setText, py::arg("newText"),
             "Set button text")
        .def("getText", &Button::getText,
             "Get button text")
        .def("getMinimumSize", &Button::getMinimumSize)
        .def("setMinimumSize", &Button::setMinimumSize, py::arg("newMinSize"))
        // 事件回调
        .def("on_click", [](Button& self, std::function<void()> callback) { return self.addEventListener("buttonClicked", std::any(callback)); }, py::arg("callback"), "Register button click callback: callback()");

    // ========================================================================
    // Label 类绑定
    // ========================================================================

    py::class_<Label, Widget, PyLabel>(m, "Label",
                                       "Text label widget")
        .def(py::init<const std::string&, Widget*>(), py::arg("text"), py::arg("parent") = nullptr,
             "Create a label with text")
        .def("draw", &Label::draw)
        .def("getText", &Label::getText,
             "Get label text")
        .def("setText", &Label::setText, py::arg("newText"),
             "Set label text")
        .def("setTextColor", &Label::setTextColor, py::arg("color"),
             "Set text foreground color")
        .def("setTextBackgroundColor", &Label::setTextBackgroundColor, py::arg("color"),
             "Set text background color")
        .def("disableTextBackgroundColor", &Label::disableTextBackgroundColor,
             "Disable custom text background color")
        .def("getMinimumSize", &Label::getMinimumSize)
        .def("setWordWrap", &Label::setWordWrap, py::arg("enable"),
             "Enable or disable word wrapping")
        .def("setAutoResize", &Label::setAutoResize, py::arg("enable"),
             "Enable or disable auto-resize to fit text")
        .def("alignTextToRowCenter", &Label::alignTextToRowCenter, py::arg("enable"),
             "Enable or disable center-aligned text");

    // ========================================================================
    // LineTextInput 类绑定
    // ========================================================================

    py::class_<LineTextInput, Widget, PyLineTextInput>(m, "LineTextInput",
                                                       "Single-line text input widget")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create a text input")
        .def("draw", &LineTextInput::draw)
        .def("getText", &LineTextInput::getText,
             "Get input text")
        .def("setText", &LineTextInput::setText, py::arg("str"),
             "Set input text")
        .def("keyboardEvent", &LineTextInput::keyboardEvent, py::arg("event"))
        .def("mouseEvent", &LineTextInput::mouseEvent, py::arg("event"))
        .def("ensureCursorVisible", &LineTextInput::ensureCursorVisible,
             "Scroll to make cursor visible")
        .def("getMinimumSize", &LineTextInput::getMinimumSize)
        // 事件回调
        .def("on_text_changed", [](LineTextInput& self, std::function<void(const std::string&)> callback) { return self.addEventListener("textChanged", std::any(callback)); }, py::arg("callback"), "Register text changed callback: callback(text)");

    // ========================================================================
    // TerminalWindow 类绑定
    // ========================================================================

    py::class_<TerminalWindow, Window, PyTerminalWindow>(m, "TerminalWindow",
                                                         "Terminal-like window with scrollable text output")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create a terminal window")
        .def("draw", &TerminalWindow::draw)
        .def("write", &TerminalWindow::write, py::arg("text"),
             "Write text to terminal output");

    // ========================================================================
    // RadioButton 类绑定
    // ========================================================================

    py::class_<RadioButton, Widget, PyRadioButton>(m, "RadioButton",
                                                   "Radio button widget (selectable option)")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create an empty radio button")
        .def(py::init<const std::string&, Widget*>(), py::arg("text"), py::arg("parent") = nullptr,
             "Create a radio button with text")
        .def("draw", &RadioButton::draw)
        .def("mouseEvent", &RadioButton::mouseEvent, py::arg("event"))
        .def("setText", &RadioButton::setText, py::arg("newText"),
             "Set radio button text")
        .def("getText", &RadioButton::getText,
             "Get radio button text")
        .def("isChecked", &RadioButton::isChecked,
             "Check if radio button is selected")
        .def("setChecked", &RadioButton::setChecked, py::arg("newChecked"),
             "Set radio button selection state")
        .def("getMinimumSize", &RadioButton::getMinimumSize)
        // 事件回调
        .def("on_checked_changed", [](RadioButton& self, std::function<void(bool)> callback) { return self.addEventListener("checkedChanged", std::any(callback)); }, py::arg("callback"), "Register checked state changed callback: callback(isChecked)");

    // ========================================================================
    // RadioButtonGroup 类绑定
    // ========================================================================

    py::class_<RadioButtonGroup>(m, "RadioButtonGroup",
                                 "Manages a group of radio buttons for mutual exclusion")
        .def(py::init<>())
        .def("addButton", &RadioButtonGroup::addButton, py::arg("button"), py::arg("id"),
             "Add a radio button to the group with an ID")
        .def("removeButton", &RadioButtonGroup::removeButton, py::arg("button"),
             "Remove a radio button from the group")
        .def("getCheckedButton", &RadioButtonGroup::getCheckedButton,
             "Get the currently checked button", py::return_value_policy::reference);

    // ========================================================================
    // ScrollWidget 类绑定
    // ========================================================================

    py::class_<ScrollWidget, Widget, PyScrollWidget>(m, "ScrollWidget",
                                                     "Scrollable container widget")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create a scroll widget")
        .def("draw", &ScrollWidget::draw)
        .def("getChildDrawableArea", &ScrollWidget::getChildDrawableArea, py::arg("childWidget"))
        .def("mouseEvent", &ScrollWidget::mouseEvent, py::arg("event"))
        .def("keyboardEvent", &ScrollWidget::keyboardEvent, py::arg("event"))
        .def("setScrollable", &ScrollWidget::setScrollable, py::arg("vertical"), py::arg("horizontal"),
             "Enable/disable vertical and horizontal scrolling")
        .def("setOffsets", &ScrollWidget::setOffsets, py::arg("verticalOffset"), py::arg("horizontalOffset"),
             "Set scroll offsets")
        .def("scrollToBottom", &ScrollWidget::scrollToBottom,
             "Scroll to the bottom");

    // ========================================================================
    // TableWidget 类绑定
    // ========================================================================

    py::class_<TableWidget, Widget, PyTableWidget>(m, "TableWidget",
                                                   "Table widget with rows and columns")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create a table widget")
        .def("draw", &TableWidget::draw)
        .def("setSize", &TableWidget::setSize, py::arg("rows"), py::arg("cols"),
             "Set table dimensions")
        .def("setCell", &TableWidget::setCell, py::arg("row"), py::arg("col"), py::arg("text"),
             "Set cell text at given position")
        .def("getMinimumSize", &TableWidget::getMinimumSize);

    // ========================================================================
    // CanvasWidget 类绑定
    // ========================================================================

    py::class_<CanvasWidget, Widget, PyCanvasWidget>(m, "CanvasWidget",
                                                     "Widget for pixel-level drawing")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create a canvas widget")
        .def("draw", &CanvasWidget::draw)
        .def("setPixel", &CanvasWidget::setPixel, py::arg("x"), py::arg("y"), py::arg("pixel"),
             "Set a pixel at given position")
        .def("clearPixel", &CanvasWidget::clearPixel, py::arg("x"), py::arg("y"),
             "Clear a pixel at given position")
        .def("setSize", &CanvasWidget::setSize, py::arg("width"), py::arg("height"),
             "Set canvas size")
        .def("clear", &CanvasWidget::clear,
             "Clear all pixels")
        .def("canvasWidth", &CanvasWidget::canvasWidth,
             "Get canvas width")
        .def("canvasHeight", &CanvasWidget::canvasHeight,
             "Get canvas height")
        .def("getMinimumSize", &CanvasWidget::getMinimumSize);

    // ========================================================================
    // HorizontalLayoutWidget 类绑定
    // ========================================================================

    py::class_<HorizontalLayoutWidget, Widget, PyHorizontalLayoutWidget>(m, "HorizontalLayoutWidget",
                                                                         "Horizontal layout container that arranges children side by side")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create a horizontal layout widget")
        .def("draw", &HorizontalLayoutWidget::draw)
        .def("getMinimumSize", &HorizontalLayoutWidget::getMinimumSize);

    // ========================================================================
    // VerticalLayoutWidget 类绑定
    // ========================================================================

    py::class_<VerticalLayoutWidget, Widget, PyVerticalLayoutWidget>(m, "VerticalLayoutWidget",
                                                                     "Vertical layout container that arranges children top to bottom")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create a vertical layout widget")
        .def("draw", &VerticalLayoutWidget::draw)
        .def("getMinimumSize", &VerticalLayoutWidget::getMinimumSize);

    // ========================================================================
    // MarginContainerWidget 类绑定
    // ========================================================================

    py::class_<MarginContainerWidget, Widget, PyMarginContainerWidget>(m, "MarginContainerWidget",
                                                                       "Container widget that adds margins around its child")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create a margin container widget")
        .def("draw", &MarginContainerWidget::draw)
        .def("getChildDrawableArea", &MarginContainerWidget::getChildDrawableArea, py::arg("childWidget"))
        .def("getMinimumSize", &MarginContainerWidget::getMinimumSize)
        .def("setMargin", &MarginContainerWidget::setMargin,
             py::arg("top"), py::arg("right"), py::arg("bottom"), py::arg("left"),
             "Set margins (top, right, bottom, left)");

    // ========================================================================
    // MaximizeContainerWidget 类绑定
    // ========================================================================

    py::class_<MaximizeContainerWidget, Widget, PyMaximizeContainerWidget>(m, "MaximizeContainerWidget",
                                                                           "Container that maximizes its child to fill available space")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create a maximize container widget")
        .def("draw", &MaximizeContainerWidget::draw)
        .def("setMaximize", &MaximizeContainerWidget::setMaximize, py::arg("width"), py::arg("height"),
             "Enable/disable width and height maximization")
        .def("setWidth", &MaximizeContainerWidget::setWidth, py::arg("width"),
             "Set fixed width (disables width maximization)")
        .def("setHeight", &MaximizeContainerWidget::setHeight, py::arg("height"),
             "Set fixed height (disables height maximization)");

    // ========================================================================
    // WindowResizeHandle 类绑定
    // ========================================================================

    py::class_<WindowResizeHandle, Widget, PyWindowResizeHandle>(m, "WindowResizeHandle",
                                                                 "Handle widget for resizing windows")
        .def(py::init<Widget*>(), py::arg("parent") = nullptr,
             "Create a window resize handle")
        .def("draw", &WindowResizeHandle::draw)
        .def("mouseEvent", &WindowResizeHandle::mouseEvent, py::arg("event"));

    // ========================================================================
    // Canvas 类绑定
    // ========================================================================

    py::class_<Canvas>(m, "Canvas",
                       "Drawing canvas for pixel manipulation")
        .def(py::init<short, short>(), py::arg("width"), py::arg("height"),
             "Create a canvas with given dimensions")
        .def("getSize", &Canvas::getSize,
             "Get canvas size as (width, height) tuple")
        .def("width", &Canvas::width,
             "Get canvas width")
        .def("height", &Canvas::height,
             "Get canvas height")
        .def("setPixel", &Canvas::setPixel, py::arg("x"), py::arg("y"), py::arg("pixel"),
             "Set a pixel at given position")
        .def("getPixel", &Canvas::getPixel, py::arg("x"), py::arg("y"),
             "Get pixel at given position", py::return_value_policy::reference_internal)
        .def("fill", py::overload_cast<Rect, const Pixel&>(&Canvas::fill),
             py::arg("rect"), py::arg("pixel"),
             "Fill a rectangle with a pixel")
        .def("fill", py::overload_cast<Rect, const Color&>(&Canvas::fill),
             py::arg("rect"), py::arg("bgColor"),
             "Fill a rectangle with a background color")
        .def("drawText", py::overload_cast<Rect, const std::string&, const Color&, const Color&>(&Canvas::drawText),
             py::arg("rect"), py::arg("text"), py::arg("fgColor"), py::arg("bgColor"),
             "Draw text with foreground and background colors")
        .def("drawText", py::overload_cast<Rect, const std::string&, const Color&>(&Canvas::drawText),
             py::arg("rect"), py::arg("text"), py::arg("fgColor"),
             "Draw text with foreground color")
        .def("drawText", py::overload_cast<Rect, const std::string&>(&Canvas::drawText),
             py::arg("rect"), py::arg("text"),
             "Draw text with default colors");

    // ========================================================================
    // Painter 类绑定
    // ========================================================================

    py::class_<Painter>(m, "Painter",
                        "High-level drawing API that clips to drawable area")
        .def(py::init<Widget*>(), py::arg("widget"),
             "Create a painter for a widget (use inside draw() override)")
        .def("setPixel", &Painter::setPixel, py::arg("x"), py::arg("y"), py::arg("pixel"),
             "Set a pixel (clipped to drawable area)")
        .def("fill", py::overload_cast<Rect, const Pixel&>(&Painter::fill),
             py::arg("rect"), py::arg("pixel"),
             "Fill a rectangle with a pixel")
        .def("fill", py::overload_cast<Rect, const Color&>(&Painter::fill),
             py::arg("rect"), py::arg("bgColor"),
             "Fill a rectangle with a background color")
        .def("drawText", py::overload_cast<Rect, const std::string_view&, const Color&, const Color&>(&Painter::drawText),
             py::arg("rect"), py::arg("text"), py::arg("fgColor"), py::arg("bgColor"),
             "Draw text with foreground and background colors")
        .def("drawText", py::overload_cast<Rect, const std::string_view&, const Color&>(&Painter::drawText),
             py::arg("rect"), py::arg("text"), py::arg("fgColor"),
             "Draw text with foreground color")
        .def("drawText", py::overload_cast<Rect, const std::vector<Utf8Char>&, const Color&, const Color&>(&Painter::drawText),
             py::arg("rect"), py::arg("u8cArray"), py::arg("fgColor"), py::arg("bgColor"),
             "Draw UTF-8 character array with foreground and background colors")
        .def("drawText", py::overload_cast<Rect, const std::vector<Utf8Char>&, const Color&>(&Painter::drawText),
             py::arg("rect"), py::arg("u8cArray"), py::arg("fgColor"),
             "Draw UTF-8 character array with foreground color")
        .def("reverseFgBgColor", &Painter::reverseFgBgColor, py::arg("x"), py::arg("y"),
             "Reverse foreground and background colors of a pixel")
        .def_static("getRealDrawableArea", &Painter::getRealDrawableArea, py::arg("widget"),
                    "Get the real drawable area for a widget");

    // ========================================================================
    // MessageBoxWindow 类绑定 (全部静态方法)
    // ========================================================================

    py::class_<MessageBoxWindow>(m, "MessageBoxWindow",
                                 "Message box dialog with static methods")
        .def_static("exec", &MessageBoxWindow::exec,
                    py::arg("title"), py::arg("message"), py::arg("buttons"), py::arg("icon"),
                    "Show message box with custom icon, returns button index")
        .def_static("question", &MessageBoxWindow::question,
                    py::arg("title"), py::arg("message"), py::arg("buttons"),
                    "Show question message box, returns button index")
        .def_static("warning", &MessageBoxWindow::warning,
                    py::arg("title"), py::arg("message"), py::arg("buttons"),
                    "Show warning message box, returns button index")
        .def_static("information", &MessageBoxWindow::information,
                    py::arg("title"), py::arg("message"), py::arg("buttons"),
                    "Show information message box, returns button index")
        .def_static("error", &MessageBoxWindow::error,
                    py::arg("title"), py::arg("message"), py::arg("buttons"),
                    "Show error message box, returns button index");

    // ========================================================================
    // Tui 类
    // ========================================================================

    py::class_<Tui>(m, "Tui",
                    "Main TUI engine class (singleton)")
        .def(py::init<>())
        .def_static("instance", &Tui::instance, py::return_value_policy::reference,
                    "Get the singleton Tui instance")
        .def("init", &Tui::init,
             "Initialize the TUI system")
        .def("deinit", &Tui::deinit,
             "Deinitialize the TUI system")
        .def("loop", &Tui::loop,
             "Run the main event loop")
        .def("processEvents", &Tui::processEvents,
             "Process pending events")
        .def("quit", &Tui::quit,
             "Request to quit the event loop")
        .def("getCanvas", &Tui::getCanvas, py::return_value_policy::reference,
             "Get the main canvas")
        .def("addRootWidget", &Tui::addRootWidget, py::arg("widget"),
             "Add a root-level widget")
        .def("removeRootWidget", &Tui::removeRootWidget, py::arg("widget"),
             "Remove a root-level widget")
        .def("width", &Tui::width,
             "Get screen width")
        .def("height", &Tui::height,
             "Get screen height")
        .def("deleteWidgetLater", &Tui::deleteWidgetLater, py::arg("widget"),
             "Schedule a widget for deletion after event processing")
        .def("focusWidget", &Tui::focusWidget, py::arg("widget"),
             "Set focus to a widget")
        .def("isFocusedWidget", &Tui::isFocusedWidget, py::arg("widget"),
             "Check if a widget is focused")
        .def("getFocusedWidget", &Tui::getFocusedWidget, py::return_value_policy::reference,
             "Get the currently focused widget")
        .def("isCtrlPressed", &Tui::isCtrlPressed,
             "Check if Ctrl key is pressed")
        .def("isAltPressed", &Tui::isAltPressed,
             "Check if Alt key is pressed")
        .def("isShiftPressed", &Tui::isShiftPressed,
             "Check if Shift key is pressed")
        .def("isMouseHoverWidget", &Tui::isMouseHoverWidget, py::arg("widget"),
             "Check if mouse is hovering over a widget")
        .def("isEventLoopShouldQuit", &Tui::isEventLoopShouldQuit,
             "Check if event loop should quit")
        .def("getRadioBtnPrintWidth", &Tui::getRadioBtnPrintWidth,
             "Get radio button print width")
        .def("getArrowBtnPrintWidth", &Tui::getArrowBtnPrintWidth,
             "Get arrow button print width");

    // ========================================================================
    // UTF-8 工具函数绑定
    // ========================================================================

    m.def("getPrintWidth", py::overload_cast<const std::string_view&>(&getPrintWidth),
          py::arg("str"),
          "Get the display width of a UTF-8 string");

    m.def("getPrintWidth", [](const Utf8Char& u8c) -> uint8_t { return getPrintWidth(u8c); }, py::arg("u8c"), "Get the display width of a single UTF-8 character");

    m.def("getPrintWidth", [](const std::vector<Utf8Char>& u8cArray) -> uint32_t { return getPrintWidth(std::span<const Utf8Char>(u8cArray)); }, py::arg("u8cArray"), "Get the display width of a UTF-8 character array");

    m.def("getUtf8Chars", &getUtf8Chars, py::arg("str"),
          "Convert a UTF-8 string to a vector of Utf8Char");

    m.def("limitStrPrintWidth", &limitStrPrintWidth,
          py::arg("str"), py::arg("width"), py::arg("beginByte") = 0,
          "Limit string to a maximum display width");

    m.def("u8cArrayToString", &u8cArrayToString, py::arg("u8cArray"),
          "Convert a Utf8Char vector back to a UTF-8 string");

    m.def("getMultiRowPrintSize", &getMultiRowPrintSize, py::arg("str"),
          "Get the display size (width, height) of a multi-row string");

    // ========================================================================
    // 全局变量绑定 (Utf8Char 常量)
    // ========================================================================

    // 注意：这些是 extern 变量，需要在模块初始化时读取
    // 由于 Utf8Char 已注册为 str 的自动转换，这里直接暴露为 str
    m.attr("RADIO_BTN_UNCHECKED") = py::cast(radioBtnUtf8Char0);  // ○
    m.attr("RADIO_BTN_CHECKED") = py::cast(radioBtnUtf8Char1);    // ●
    m.attr("ARROW_LEFT") = py::cast(arrowBtnUtf8Char0);           // ←
    m.attr("ARROW_UP") = py::cast(arrowBtnUtf8Char1);             // ↑
    m.attr("ARROW_RIGHT") = py::cast(arrowBtnUtf8Char2);          // →
    m.attr("ARROW_DOWN") = py::cast(arrowBtnUtf8Char3);           // ↓
}
