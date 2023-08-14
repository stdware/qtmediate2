include(${CMAKE_CURRENT_LIST_DIR}/utils.cmake)

# No Icon
if(NOT DEFINED RC_ICON_PATH)
    set(RC_ICON_COMMENT "//")
    set(RC_ICON_PATH)
endif()

# Metadata
if(NOT DEFINED RC_PROJECT_NAME)
    set(RC_PROJECT_NAME ${PROJECT_NAME})
endif()

if(NOT DEFINED RC_APPLICATION_NAME)
    set(RC_APPLICATION_NAME ${RC_PROJECT_NAME})
endif()

if(NOT DEFINED RC_VERSION_STRING)
    set(RC_VERSION_STRING ${PROJECT_VERSION})
endif()

if(NOT DEFINED RC_DESCRIPTION)
    set(RC_DESCRIPTION "Qt Intermediate Patching Framework")
endif()

if(NOT DEFINED RC_COPYRIGHT)
    set(RC_COPYRIGHT "Copyright 2022-2023 SineStriker")
endif()

parse_version(_version ${RC_VERSION_STRING})
set(RC_VERSION ${_version_1},${_version_2},${_version_3},${_version_4})

# Generate rc file
set(_rc_path ${CMAKE_CURRENT_BINARY_DIR}/${RC_PROJECT_NAME}_res.rc)
configure_file(${CMAKE_CURRENT_LIST_DIR}/WinResource.rc.in ${_rc_path} @ONLY)

# Add source
target_sources(${RC_PROJECT_NAME} PRIVATE ${_rc_path})
