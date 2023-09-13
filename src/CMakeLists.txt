# ----------------------------------
# CMake API
# ----------------------------------
function(qtmediate_add_library _target _type)
    add_library(${_target} ${_type} ${ARGN})

    set(_gen_include_dir ${QTMEDIATE_GENERATED_INCLUDE_DIR}/${_target})

    if(NOT EXISTS ${_gen_include_dir})
        qtmediate_gen_include(. ${_gen_include_dir})
    endif()

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

        install(CODE "
            include(${QTMEDIATE_MODULES_DIR}/qtmediateMacros.cmake)
            qtmediate_gen_include(
                \"${CMAKE_CURRENT_SOURCE_DIR}\"
                \"${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/${QTMEDIATE_INSTALL_NAME}/${_target}\"
                COPY CLEAN
            )
        "
        )
    endif()

    target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${QTMEDIATE_GENERATED_INCLUDE_DIR}>"
    )

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

# ----------------------------------
# Main Project
# ----------------------------------
set(RC_DESCRIPTION "Qt with custom implementations")
set(RC_COPYRIGHT "Copyright 2019-2023 SineStriker")

set(QTMEDIATE_GENERATED_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/../include)

include(${QTMEDIATE_MODULES_DIR}/qtmediateMacros.cmake)

add_subdirectory(corelib)

add_subdirectory(gui)

add_subdirectory(widgets)

add_subdirectory(widgettools)

add_subdirectory(plugins)

if(QTMEDIATE_BUILD_DOCUMENTATIONS)
    if(NOT DOXYGEN_EXECUTABLE)
        find_package(Doxygen REQUIRED)
    endif()

    include(${QTMEDIATE_MODULES_DIR}/doxygen/doxygen.cmake)

    set(_install_options)

    if(QTMEDIATE_INSTALL)
        set(_install_options INSTALL_DIR share/doc/${QTMEDIATE_INSTALL_NAME})
    endif()

    set(_doc_targets QMCore QMGui QMWidgets QMWidgetTools)

    set(QTMEDIATE_DOXYGEN_TAGFILE ${CMAKE_BUILD_SHARE_DIR}/doc/${QTMEDIATE_INSTALL_NAME}/qtmediate_tagfile.xml
        CACHE FILEPATH "qtmediate doxygen tag file path" FORCE
    )

    setup_doxygen_command(qtmediate_RunDoxygen
        NAME "Qt Mediate"
        DESCRIPTION "${RC_DESCRIPTION}"
        MDFILE ../README.md
        OUTPUT_DIR ${CMAKE_BUILD_SHARE_DIR}/doc/${QTMEDIATE_INSTALL_NAME}
        INPUT corelib gui widgets widgettools
        TARGETS ${_doc_targets}
        DEPENDS ${_doc_targets}
        NO_EXPAND_MACROS Q_OBJECT Q_GADGET Q_DECLARE_TR_FUNCTIONS
        COMPILE_DEFINITIONS Q_SIGNALS=Q_SIGNALS Q_SLOTS=Q_SLOTS
        GENERATE_TAGFILE "${QTMEDIATE_DOXYGEN_TAGFILE}"
        ${_install_options}
    )
endif()

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

    # Install cmake modules
    install(DIRECTORY ${QTMEDIATE_MODULES_DIR}/
        DESTINATION ${_install_dir}/cmake
        PATTERN "__pycache__" EXCLUDE
    )
endif()