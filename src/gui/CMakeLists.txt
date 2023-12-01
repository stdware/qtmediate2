project(QMGui
    VERSION ${QTMEDIATE_VERSION}
    LANGUAGES CXX
)

# Configure target
file(GLOB_RECURSE _src *.h *.hpp *.cpp *.cc)

qtmediate_add_library(${PROJECT_NAME} AUTOGEN
    SOURCES ${_src}
    LINKS QMCore
    INCLUDE_PRIVATE Global Kernel Painting Svgx Tools
    QT_LINKS Core Gui
    QT_INCLUDE_PRIVATE Core Gui
    SKIP_AUTOMOC Tools
)