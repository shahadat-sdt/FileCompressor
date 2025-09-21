cmake_minimum_required(VERSION 3.16)
project(FileCompressor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(FileCompressor
        main.cpp
        mainwindow.cpp
        mainwindow.h
        editorwindow.cpp
        editorwindow.h
        huffman.cpp
        huffman.h
        advancedDetailsWindow.cpp
)

target_link_libraries(FileCompressor
        Qt6::Widgets
)
