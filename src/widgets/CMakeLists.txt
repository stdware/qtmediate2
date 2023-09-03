project(QMWidgets
    VERSION ${QTMEDIATE_VERSION}
    LANGUAGES CXX
)

set(CMAKE_AUTOUIC on)
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

# Platform libs
set(_platform_libs)

if(WIN32)
    set(_platform_libs dwmapi)
else()
    set(_platform_libs dl)
endif()

# Configure target
file(GLOB_RECURSE _src *.h *.hpp *.cpp *.cc)

qtmediate_add_library(${PROJECT_NAME} SHARED ${_src})

add_library(qtmediate::Widgets ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME Widget)

target_compile_definitions(${PROJECT_NAME} PRIVATE QMWIDGETS_LIBRARY)

# Link
target_link_libraries(${PROJECT_NAME} PUBLIC QMGui)

target_link_libraries(${PROJECT_NAME} PRIVATE ${_platform_libs})

qtmediate_link_qt_libraries(${PROJECT_NAME} PUBLIC
    Core Gui Widgets Svg
)

# Include
qtmediate_include_directories(${PROJECT_NAME}
    Global Kernel Layout Widgets .
)

qtmediate_include_qt_private(${PROJECT_NAME} PRIVATE
    Core Gui Widgets
)

# Extra
qtmediate_install_headers(.)