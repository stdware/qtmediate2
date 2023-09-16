project(nativewindow
    VERSION 0.0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find FramelessHelper
find_package(FramelessHelper QUIET COMPONENTS Core Widgets)

if(NOT TARGET FramelessHelper::Widgets)
    message(WARNING "FramelessHelper not found, skip build ${PROJECT_NAME}")
    return()
endif()

# Configure target
file(GLOB_RECURSE _src *.h *.hpp *.cpp *.cc)

qtmediate_add_plugin(${PROJECT_NAME} ${PLUGIN_CATEGORY}
    SOURCES ${_src} Res/NativeWindow_res.qrc
    LINKS QMWidgetTools
    LINKS_PRIVATE FramelessHelper::Widgets
    QT_LINKS Core Gui Widgets
    INCLUDE_PRIVATE .
    QT_INCLUDE_PRIVATE Core Gui
)