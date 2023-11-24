project(QMWidgetTools
    VERSION ${QTMEDIATE_VERSION}
    LANGUAGES CXX
)

# Configure target
file(GLOB_RECURSE _src *.h *.hpp *.cpp *.cc)

qtmediate_add_library(${PROJECT_NAME} AUTOGEN
    PREFIX QMWTOOLS
    SOURCES ${_src}
    LINKS QMWidgets
    INCLUDE_PRIVATE Dialogs Global Tools Widgets Windows
    QT_LINKS Core Gui Widgets
    QT_INCLUDE_PRIVATE Core Gui Widgets
)