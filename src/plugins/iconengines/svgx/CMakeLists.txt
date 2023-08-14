project(svgxicon
    VERSION 0.0.0.1
    LANGUAGES CXX
)

set(CMAKE_AUTOUIC on)
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

# Configure target
file(GLOB_RECURSE _src *.h *.hpp *.cpp *.cc)

qtmediate_add_plugin(${PROJECT_NAME} ${PLUGIN_CATEGORY} ${_src})

# Link
target_link_libraries(${PROJECT_NAME} PUBLIC QMGui)

qtmediate_link_qt_libraries(${PROJECT_NAME} PUBLIC
    Core Gui Svg
)

# Include
target_include_directories(${PROJECT_NAME} PRIVATE .)

qtmediate_include_qt_private(${PROJECT_NAME} PRIVATE
    Core Gui
)