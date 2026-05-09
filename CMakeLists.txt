cmake_minimum_required(VERSION 3.14)
project(TextGui VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES

    src/tui/tui.cpp
    src/tui/canvas.cpp
    src/tui/utf8Utils.cpp
    src/tui/painter.cpp
    src/tui/label.cpp
    src/tui/widget.cpp
    src/tui/window.cpp
    src/tui/button.cpp
    src/tui/terminalWindow.cpp
    src/tui/lineTextInput.cpp
    src/tui/scrollWidget.cpp
    src/tui/verticalLayoutWidget.cpp
    src/tui/horizontalLayoutWidget.cpp
    src/tui/maximizeContainerWidget.cpp
    src/tui/marginContainerWidget.cpp
    src/tui/messagebox.cpp
    src/tui/canvasWidget.cpp
    src/tui/tableWidget.cpp
    src/tui/windowResizeHandle.cpp
    src/tui/dropdown.cpp
    src/tui/radioButton.cpp
    src/tui/radioButtonGroup.cpp
)

if(WIN32)
    list(APPEND SOURCES src/tui/tui_win.cpp)
elseif(UNIX AND NOT APPLE)
    list(APPEND SOURCES src/tui/tui_linux.cpp)
else()
    message(FATAL_ERROR "Unsupported platform: only Windows and Linux are supported")
endif()

add_library(TextGui STATIC ${SOURCES})
add_library(TextGui::TextGui ALIAS TextGui)

target_compile_features(TextGui PUBLIC cxx_std_23)

target_include_directories(TextGui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/tui
)

if(UNIX AND NOT APPLE)
    target_link_libraries(TextGui PRIVATE pthread)
endif()

option(TEXTGUI_BUILD_EXAMPLES "Build example programs" ON)
if(TEXTGUI_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
