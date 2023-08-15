project(tst_frameless)

set(CMAKE_AUTOUIC on)
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

file(GLOB_RECURSE _src *.h *.cpp)

add_executable(${PROJECT_NAME} ${_src} frameless.manifest)

target_link_libraries(${PROJECT_NAME} PRIVATE QMWidgetTools)

qtmediate_link_qt_libraries(${PROJECT_NAME} PUBLIC
    Core Gui Widgets Svg
)