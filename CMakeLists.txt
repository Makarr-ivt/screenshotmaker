cmake_minimum_required(VERSION 3.16)
project(screenshotmaker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SOURCES
    main.cpp
    mainwindow.cpp
    screenshotcapturer.cpp
)

set(HEADERS
    mainwindow.h
    screenshotcapturer.h
)

set(FORMS
    mainwindow.ui
)

add_executable(screenshotmaker ${SOURCES} ${HEADERS} ${FORMS})
target_link_libraries(screenshotmaker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(WIN32)
    set_target_properties(screenshotmaker PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
