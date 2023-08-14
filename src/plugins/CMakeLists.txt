macro(_init_category _category)
    set(PLUGIN_CATEGORY ${_category})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/${PLUGIN_CATEGORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/plugins/${PLUGIN_CATEGORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/plugins/${PLUGIN_CATEGORY})
endmacro()

add_subdirectory(iconengines)
add_subdirectory(windowhandles)