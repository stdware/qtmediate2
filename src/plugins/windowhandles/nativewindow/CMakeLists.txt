project(nativewindow
    VERSION 0.0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC on)
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

find_package(FramelessHelper QUIET COMPONENTS Core Widgets)

if(NOT TARGET FramelessHelper::Widgets)
    message(WARNING "FramelessHelper not found, skip build ${PROJECT_NAME}")
    return()
endif()

# Configure target
file(GLOB_RECURSE _src *.h *.hpp *.cpp *.cc)

qtmediate_add_plugin(${PROJECT_NAME} ${PLUGIN_CATEGORY} ${_src} Res/NativeWindow_res.qrc)

# Link
target_link_libraries(${PROJECT_NAME} PRIVATE QMWidgetTools FramelessHelper::Widgets)

qtmediate_link_qt_libraries(${PROJECT_NAME} PUBLIC
    Widgets
)

# Include
target_include_directories(${PROJECT_NAME} PRIVATE .)

qtmediate_include_qt_private(${PROJECT_NAME} PRIVATE
    Core Gui
)