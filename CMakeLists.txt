cmake_minimum_required(VERSION 3.14)

project(game_of_life_qtquick LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Core Quick REQUIRED)
find_package(Qt6 COMPONENTS Core Quick REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        TableModel.cpp
        TableModel.hpp
        Cell.cpp
        Cell.hpp
        qml.qrc
)

qt_add_executable(game_of_life_qtquick
    ${PROJECT_SOURCES}
)

target_compile_definitions(game_of_life_qtquick PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(game_of_life_qtquick PRIVATE Qt6::Core Qt6::Quick)
