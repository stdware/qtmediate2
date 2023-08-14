if(QTMEDIATE_BUILD_TRANSLATIONS)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS LinguistTools REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS LinguistTools REQUIRED)

    set(_ts_files)
    set(_qm_files)
    set(_qm_dir ${CMAKE_BINARY_DIR}/share/qtmediate/translations)

    foreach(_item ${QTMEDIATE_TRANSLATIONS})
        list(APPEND _ts_files ${CMAKE_CURRENT_SOURCE_DIR}/translations/qtmediate_${_item}.ts)
    endforeach()

    set_source_files_properties(${_ts_files}
        PROPERTIES OUTPUT_LOCATION ${_qm_dir}
    )

    get_property(_targets GLOBAL PROPERTY TRANSLATE_TARGETS)
    set(_src)

    foreach(_item ${_targets})
        get_target_property(_files ${_item} SOURCES)
        list(FILTER _files INCLUDE REGEX ".+\\.(cpp|cc)")
        list(FILTER _files EXCLUDE REGEX "(qasc|moc)_.+")
        list(APPEND _src ${_files})
    endforeach()

    qt5_create_translation(_qm_files ${_ts_files} ${_src})

    add_custom_target(lupdate_task DEPENDS ${_ts_files})
    add_custom_target(lrelease_task DEPENDS ${_qm_files})

    if(QTMEDIATE_INSTALL)
        install(CODE "
        execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target lupdate_task)
        execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target lrelease_task)
    ")
        install(DIRECTORY ${_qm_dir}/
            DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${QTMEDIATE_INSTALL_NAME}/translations
            FILES_MATCHING PATTERN "*.qm"
        )
    endif()
endif()