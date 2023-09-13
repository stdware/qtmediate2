project(QMGui
    VERSION ${QTMEDIATE_VERSION}
    LANGUAGES CXX
)

set(CMAKE_AUTOUIC on)
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

# Configure target
file(GLOB_RECURSE _src *.h *.hpp *.cpp *.cc)

qtmediate_add_library(${PROJECT_NAME} SHARED ${_src})

add_library(qtmediate::Gui ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME Gui)

target_compile_definitions(${PROJECT_NAME} PRIVATE QMGUI_LIBRARY)

# Link
target_link_libraries(${PROJECT_NAME} PUBLIC
    QMCore
)

qtmediate_link_qt_libraries(${PROJECT_NAME} PUBLIC
    Core Gui
)

# Include
qtmediate_include_qt_private(${PROJECT_NAME} PRIVATE
    Core Gui
)

target_include_directories(${PROJECT_NAME} PRIVATE
    Global Kernel Painting Svgx Tools
)

# Extra
qtmediate_dir_skip_automoc(Tools)