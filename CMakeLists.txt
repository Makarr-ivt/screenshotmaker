cmake_minimum_required(VERSION 3.16)
project(screenshotmaker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(CAPTURE_HEADERS
    capture/icapturestrategy.h
    capture/fullscreencapturestrategy.h
    capture/capturemanager.h
    capture/rectangleselectionoverlay.h
)
set(CAPTURE_SOURCES
    capture/fullscreencapturestrategy.cpp
    capture/capturemanager.cpp
    capture/rectangleselectionoverlay.cpp
)

set(SERVICES_HEADERS
    services/fileexportservice.h
    services/clipboardservice.h
)
set(SERVICES_SOURCES
    services/fileexportservice.cpp
    services/clipboardservice.cpp
)

set(CONTROLLER_HEADERS
    controllers/screenshotcontroller.h
)
set(CONTROLLER_SOURCES
    controllers/screenshotcontroller.cpp
)

set(APP_HEADERS
    app/mainwindow.h
    app/screenshotviewer.h
)
set(APP_SOURCES
    app/main.cpp
    app/mainwindow.cpp
    app/screenshotviewer.cpp
)

set(APP_FORMS
    app/mainwindow.ui
)

add_executable(screenshotmaker
    ${APP_SOURCES}
    ${APP_HEADERS}
    ${APP_FORMS}

    ${CAPTURE_SOURCES}
    ${CAPTURE_HEADERS}

    ${SERVICES_SOURCES}
    ${SERVICES_HEADERS}

    ${CONTROLLER_SOURCES}
    ${CONTROLLER_HEADERS}
)

target_include_directories(screenshotmaker PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/app
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/capture
    ${CMAKE_CURRENT_SOURCE_DIR}/services
)


target_link_libraries(screenshotmaker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Windows: запуск приложения без консольного окна
if(WIN32)
    set_target_properties(screenshotmaker PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# Выходной каталог
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
