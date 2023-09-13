if(NOT DEFINED QTMEIATE_MODULES_DIR)
    set(QTMEIATE_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

#[[
    Skip CMAKE_AUTOMOC for all source files in directory.

    qtmediate_dir_skip_automoc()
]] #
macro(qtmediate_dir_skip_automoc)
    foreach(_item ${ARGN})
        file(GLOB _src ${_item}/*.h ${_item}/*.cpp ${_item}/*.cc)
        set_source_files_properties(
            ${_src} PROPERTIES SKIP_AUTOMOC ON
        )
    endforeach()
endmacro()

#[[
    Link Qt libraries.

    qtmediate_link_qt_libraries(<target> scope)
#]]
macro(qtmediate_link_qt_libraries _target _scope)
    foreach(_module ${ARGN})
        find_package(QT NAMES Qt6 Qt5 COMPONENTS ${_module} REQUIRED)
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${_module} REQUIRED)
        target_link_libraries(${_target} ${_scope} Qt${QT_VERSION_MAJOR}::${_module})
    endforeach()
endmacro()

#[[
    Include Qt private header directories.

    qtmediate_link_qt_libraries(<target> scope)
#]]
macro(qtmediate_include_qt_private _target _scope)
    foreach(_module ${ARGN})
        target_include_directories(${_target} ${_scope} ${Qt${QT_VERSION_MAJOR}${_module}_PRIVATE_INCLUDE_DIRS})
    endforeach()
endmacro()

#[[
    Generate reference include directories.

    qtmediate_gen_include(<src> <dest>
        [COPY] [CLEAN]
    )
#]]
function(qtmediate_gen_include _src_dir _dest_dir)
    set(options)
    set(oneValueArgs COPY CLEAN)
    set(multiValueArgs)
    cmake_parse_arguments(GEN_INCLUDE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT IS_ABSOLUTE ${_src_dir})
        get_filename_component(_src_dir ${_src_dir} ABSOLUTE)
    else()
        set(_src_dir ${_src_dir})
    endif()

    if(NOT IS_ABSOLUTE ${_dest_dir})
        get_filename_component(_dest_dir ${_dest_dir} ABSOLUTE)
    else()
        set(_dest_dir ${_dest_dir})
    endif()

    if(IS_DIRECTORY ${_src_dir})
        file(GLOB_RECURSE header_files ${_src_dir}/*.h ${_src_dir}/*.hpp)

        if(GEN_INCLUDE_CLEAN AND EXISTS ${_dest_dir})
            if(IS_DIRECTORY ${_dest_dir})
                file(REMOVE_RECURSE ${_dest_dir})
            else()
                file(REMOVE ${_dest_dir})
            endif()
        endif()

        foreach(_file ${header_files})
            get_filename_component(file_name ${_file} NAME)

            if(file_name MATCHES "_p\\.")
                set(_dir ${_dest_dir}/private)
            else()
                set(_dir ${_dest_dir})
            endif()

            if(NOT EXISTS ${_dir})
                file(MAKE_DIRECTORY ${_dir})
            endif()

            file(RELATIVE_PATH rel_path ${_dir} ${_file})
            string(REPLACE "\\" "/" rel_path ${rel_path})
            set(new_file ${_dir}/${file_name})

            if(GEN_INCLUDE_COPY)
                file(COPY ${_file} DESTINATION ${_dir})
            else()
                file(WRITE ${new_file} "#include \"${rel_path}\"\n")
            endif()
        endforeach()
    else()
        message(FATAL_ERROR "qtmediate_gen_include_files: Source directory doesn't exist.")
    endif()
endfunction()
