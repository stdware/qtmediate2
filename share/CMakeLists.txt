if(QTMEDIATE_BUILD_TRANSLATIONS)
    qm_import(Translate)
    qm_find_qt(LinguistTools)

    add_custom_target(qtmediate_lupdate_task)
    add_custom_target(qtmediate_lrelease_task)

    # qtbase
    file(GLOB _ts_files translations/qtbase_*.ts)
    set(_qm_dir ${CMAKE_BUILD_SHARE_DIR}/${QTMEDIATE_INSTALL_NAME}/translations)
    qm_add_translation(qtbase_translations
        TS_FILES ${_ts_files}
        QM_DIR ${_qm_dir}
        QM_DEPENDS qtmediate_lrelease_task
    )

    # qtmediate
    get_property(_targets GLOBAL PROPERTY QTMEDIATE_TRANSLATE_TARGETS)
    qm_add_translation(qtmediate_translations
        LOCALES ja_JP zh_CN zh_HK
        PREFIX qtmediate
        TARGETS ${_targets}
        TS_DIR translations
        QM_DIR ${_qm_dir}
        TS_DEPENDS qtmediate_lupdate_task
        QM_DEPENDS qtmediate_lrelease_task
    )

    if(QTMEDIATE_INSTALL)
        install(CODE "
            execute_process(COMMAND \"${CMAKE_COMMAND}\" --build \"${CMAKE_BINARY_DIR}\" --target qtmediate_lupdate_task)
            execute_process(COMMAND \"${CMAKE_COMMAND}\" --build \"${CMAKE_BINARY_DIR}\" --target qtmediate_lrelease_task)
        ")
        install(DIRECTORY ${_qm_dir}/
            DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${QTMEDIATE_INSTALL_NAME}/translations
            FILES_MATCHING PATTERN "*.qm"
        )
    endif()
endif()