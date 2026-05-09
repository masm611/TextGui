set(SOURCES

    main.cpp
    controlWindow.cpp
    awardRecord.cpp
    findRecordWindow.cpp
    infoTableWindow.cpp
    newRecordWindow.cpp
)

add_executable(TextGuiExample ${SOURCES})
target_link_libraries(TextGuiExample PRIVATE TextGui::TextGui)

# 复制data.txt到生成目录
add_custom_command(TARGET TextGuiExample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/data.txt
    $<TARGET_FILE_DIR:TextGuiExample>/data.txt
)