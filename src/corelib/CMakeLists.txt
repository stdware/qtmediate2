project(QMCore
    VERSION ${QTMEDIATE_VERSION}
    LANGUAGES CXX
)

set(CMAKE_AUTOUIC on)
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

# Platform libs
set(_platform_libs)

if(APPLE)
    list(APPEND _platform_libs "-framework CoreFoundation")
endif()

if(NOT WIN32)
    list(APPEND _platform_libs dl)
endif()

# Configure target
file(GLOB_RECURSE _src *.h *.hpp *.cpp *.cc)

qtmediate_add_library(${PROJECT_NAME} SHARED ${_src})

add_library(qtmediate::Core ALIAS ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE QMCORE_LIBRARY)

# Link
target_link_libraries(${PROJECT_NAME} PRIVATE ${_platform_libs})

qtmediate_link_qt_libraries(${PROJECT_NAME} PUBLIC
    Core
)

# Include
qtmediate_include_directories(${PROJECT_NAME}
    Collections Global Kernel Serialization Text .
)

qtmediate_include_qt_private(${PROJECT_NAME} PRIVATE
    Core
)

# Extra
qtmediate_dir_skip_automoc(Collections Serialization Text)

qtmediate_install_headers(.)