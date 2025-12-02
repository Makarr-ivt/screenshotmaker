cmake_minimum_required(VERSION 3.16)
project(screenshotmaker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматическое подключение MOC, UIC и RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Поиск Qt5 или Qt6
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Исходные файлы
set(SOURCES
    main.cpp
    mainwindow.cpp
)

# Заголовочные файлы
set(HEADERS
    mainwindow.h
)

# Файлы форм
set(FORMS
    Forms/mainwindow.ui
)

# Создание исполняемого файла
add_executable(screenshotmaker ${SOURCES} ${HEADERS} ${FORMS})

# Подключение Qt
target_link_libraries(screenshotmaker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Установка свойств для Windows (если нужно)
if(WIN32)
    set_target_properties(screenshotmaker PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
