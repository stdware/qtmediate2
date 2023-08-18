# Qt Mediate

Qt with custom implementations, the name is taken from "intermediate" because it extends many features by using a lot of workarounds without modifying any of the Qt source code.

## Dependencies

+ Qt 5.15.2 (Maybe work with 5.15.X)

+ [FramelessHelper](https://github.com/wangwenx190/framelesshelper)

## Build & Install

```sh
cmake -B build -G Ninja \
    -DCMAKE_INSTALL_PREFIX:STRING=<dir> \
    -DQTMEDIATE_BUILD_TESTS:BOOL=FALSE

cmake --build build --target all

cmake --build build --target install
```