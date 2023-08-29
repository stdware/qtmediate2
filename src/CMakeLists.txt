# ----------------------------------
# CMake API
# ----------------------------------
function(qtmediate_add_library _target _type)
    add_library(${_target} ${_type} ${ARGN})

    if(WIN32)
        set(RC_VERSION_STRING ${QTMEDIATE_INSTALL_NAME})
        include(${QTMEDIATE_MODULES_DIR}/winrc.cmake)
    endif()

    if(QTMEDIATE_INSTALL)
        install(TARGETS ${_target}
            EXPORT ${QTMEDIATE_INSTALL_NAME}Targets
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" OPTIONAL
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" OPTIONAL
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" OPTIONAL
        )

        target_include_directories(${PROJECT_NAME} PUBLIC
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${QTMEDIATE_INSTALL_NAME}>"
        )
    endif()

    set_property(GLOBAL APPEND PROPERTY QTMEDIATE_TRANSLATE_TARGETS ${_target})
endfunction()

function(qtmediate_add_plugin _target _category)
    add_library(${_target} SHARED ${ARGN})
    add_library(qtmediate::${_target} ALIAS ${_target})

    if(WIN32)
        set(RC_VERSION_STRING ${QTMEDIATE_VERSION})
        include(${QTMEDIATE_MODULES_DIR}/winrc.cmake)
    endif()

    set(_output_dir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${QTMEDIATE_INSTALL_NAME}/plugins/${_category})
    set_target_properties(${_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${_output_dir}
        ARCHIVE_OUTPUT_DIRECTORY ${_output_dir}
        LIBRARY_OUTPUT_DIRECTORY ${_output_dir}
    )

    if(QTMEDIATE_INSTALL)
        install(TARGETS ${_target}
            RUNTIME DESTINATION "${CMAKE_INSTALL_LIBDIR}/${QTMEDIATE_INSTALL_NAME}/plugins/${_category}" OPTIONAL
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/${QTMEDIATE_INSTALL_NAME}/plugins/${_category}" OPTIONAL
        )
    endif()

    set_property(GLOBAL APPEND PROPERTY TRANSLATE_TARGETS ${_target})
endfunction()

function(qtmediate_include_directories _target)
    foreach(_dir ${ARGN})
        get_filename_component(_abs_dir ${_dir} ABSOLUTE)
        file(RELATIVE_PATH _rel_path ${QTMEDIATE_SOURCE_DIR} ${_abs_dir})
        target_include_directories(${_target} PUBLIC
            "$<BUILD_INTERFACE:${_abs_dir}>"
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${QTMEDIATE_INSTALL_NAME}/${_rel_path}>"
        )
    endforeach()
endfunction()

macro(qtmediate_install_headers _dir)
    if(QTMEDIATE_INSTALL)
        get_filename_component(_abs_dir ${_dir} ABSOLUTE)
        file(RELATIVE_PATH _rel_path ${QTMEDIATE_SOURCE_DIR} ${_abs_dir})
        install(DIRECTORY ${_abs_dir}/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${QTMEDIATE_INSTALL_NAME}/${_rel_path}
            FILES_MATCHING PATTERN "*.h"
        )
    endif()
endmacro()

macro(qtmediate_dir_skip_automoc)
    foreach(_item ${ARGN})
        file(GLOB _src ${_item}/*.h ${_item}/*.cpp ${_item}/*.cc)
        set_source_files_properties(
            ${_src} PROPERTIES SKIP_AUTOMOC ON
        )
    endforeach()
endmacro()

macro(qtmediate_link_qt_libraries _target _scope)
    foreach(_module ${ARGN})
        find_package(QT NAMES Qt6 Qt5 COMPONENTS ${_module} REQUIRED)
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${_module} REQUIRED)
        target_link_libraries(${_target} ${_scope} Qt${QT_VERSION_MAJOR}::${_module})
    endforeach()
endmacro()

macro(qtmediate_include_qt_private _target _scope)
    foreach(_module ${ARGN})
        target_include_directories(${_target} ${_scope} ${Qt${QT_VERSION_MAJOR}${_module}_PRIVATE_INCLUDE_DIRS})
    endforeach()
endmacro()

# ----------------------------------
# Main Project
# ----------------------------------
add_subdirectory(corelib)

add_subdirectory(gui)

add_subdirectory(widgets)

add_subdirectory(widgettools)

add_subdirectory(plugins)

if(QTMEDIATE_INSTALL)
    # Add install target
    set(_install_dir ${CMAKE_INSTALL_LIBDIR}/cmake/${QTMEDIATE_INSTALL_NAME})

    # Add version file
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${QTMEDIATE_INSTALL_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    # Add configuration file
    configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/${QTMEDIATE_INSTALL_NAME}Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/${QTMEDIATE_INSTALL_NAME}Config.cmake"
        INSTALL_DESTINATION ${_install_dir}
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )

    # Install cmake files
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${QTMEDIATE_INSTALL_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${QTMEDIATE_INSTALL_NAME}ConfigVersion.cmake"
        DESTINATION ${_install_dir}
    )

    # Install cmake targets files
    install(EXPORT ${QTMEDIATE_INSTALL_NAME}Targets
        FILE "${QTMEDIATE_INSTALL_NAME}Targets.cmake"
        NAMESPACE qtmediate::
        DESTINATION ${_install_dir}
    )
endif()