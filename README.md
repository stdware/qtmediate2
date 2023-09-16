# Qt Mediate

Qt Intermediate Extension Framework.

The name is taken from "intermediate" because it extends many features by using a lot of workarounds without modifying any of the Qt source code.

## Dependencies

+ Qt 5.15.2 (Maybe work with 5.15.X)
+ [qtmediate-cmake-modules](https://github.com/SineStriker/qtmediate-cmake-modules)
+ [FramelessHelper](https://github.com/wangwenx190/framelesshelper)

## Extension to Qt5

+ High DPI displays
+ Switching translation
+ Support user type in stylesheets
+ Support SVG `currentColor` extension
+ Provide a registrable, extensible theme system
+ Provide more custom controls

## Build & Install

```sh
cmake -B build -G Ninja \
    -DCMAKE_INSTALL_PREFIX:STRING=<dir> \
    -DQTMEDIATE_CMAKE_MODULES_DIR:STRING=<dir> \
    -DQTMEDIATE_BUILD_TESTS:BOOL=FALSE

cmake --build build --target all

cmake --build build --target install
```