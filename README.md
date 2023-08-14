# Qt Mediate

Qt with custom implementations

## Dependencies

+ Qt 5.15.2

+ [FramelessHelper](https://github.com/wangwenx190/framelesshelper)

## Build & Install

```sh
cmake -B build -G Ninja \
    -DCMAKE_INSTALL_PREFIX:STRING=<dir> \
    -DQTMEDIATE_BUILD_TESTS:BOOL=FALSE

cmake --build build --target all

cmake --build build --target install
```