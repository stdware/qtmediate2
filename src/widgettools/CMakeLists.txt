project(QMWidgetTools
    VERSION ${QTMEDIATE_VERSION}
    LANGUAGES CXX
)

set(CMAKE_AUTOUIC on)
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

# Configure target
file(GLOB_RECURSE _src *.h *.hpp *.cpp *.cc)

qtmediate_add_library(${PROJECT_NAME} ${_src})

target_compile_definitions(${PROJECT_NAME} PRIVATE QMWTOOLS_LIBRARY)

# Link
target_link_libraries(${PROJECT_NAME} PUBLIC QMWidgets)

qtmediate_link_qt_libraries(${PROJECT_NAME} PUBLIC
    Core Gui Widgets
)

# Include
qtmediate_include_directories(${PROJECT_NAME} PUBLIC
    Dialogs Global Tools Widgets Windows .
)

qtmediate_include_qt_private(${PROJECT_NAME} PRIVATE
    Core Gui Widgets
)

# Extra
qtmediate_install_headers(.)