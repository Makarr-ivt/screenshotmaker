cmake_minimum_required(VERSION 3.16)
project(screenshotmaker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Поддиректории новой архитектуры
set(CORE_SOURCES
    core/screenshotcontext.cpp
)

set(CAPTURE_SOURCES
    capture/fullscreencapturestrategy.cpp
    capture/rectanglecapturestrategy.cpp
    capture/capturemanager.cpp
)

set(SERVICES_SOURCES
    services/fileexportservice.cpp
    services/clipboardservice.cpp
    services/imageprocessingservice.cpp
)

# Все исходные файлы
set(SOURCES
    main.cpp
    mainwindow.cpp
    ${CORE_SOURCES}
    ${CAPTURE_SOURCES}
    ${SERVICES_SOURCES}
)

set(HEADERS
    mainwindow.h
    core/screenshotcontext.h
    core/editcommand.h
    capture/icapturestrategy.h
    capture/fullscreencapturestrategy.h
    capture/rectanglecapturestrategy.h
    capture/capturemanager.h
    services/fileexportservice.h
    services/clipboardservice.h
    services/imageprocessingservice.h
)

set(FORMS
    mainwindow.ui
)

add_executable(screenshotmaker ${SOURCES} ${HEADERS} ${FORMS})
target_link_libraries(screenshotmaker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(WIN32)
    set_target_properties(screenshotmaker PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# Устанавливаем выходной каталог
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
