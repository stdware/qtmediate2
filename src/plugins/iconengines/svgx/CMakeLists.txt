project(svgxicon
    VERSION 0.0.0.1
    LANGUAGES CXX
)

# Configure target
file(GLOB_RECURSE _src *.h *.hpp *.cpp *.cc)

qtmediate_add_plugin(${PROJECT_NAME} ${PLUGIN_CATEGORY}
    SOURCES ${_src}
    LINKS QMGui
    QT_LINKS Core Gui Svg
    INCLUDE_PRIVATE .
    QT_INCLUDE_PRIVATE Core Gui
)