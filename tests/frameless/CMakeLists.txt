project(tst_frameless)

set(CMAKE_AUTOUIC on)
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

file(GLOB_RECURSE _src *.h *.cpp)

add_executable(${PROJECT_NAME})

qm_configure_target(${PROJECT_NAME}
    SOURCES ${_src} frameless.manifest
    QT_LINKS Core Gui Widgets Svg
    LINKS QMWidgetTools
)