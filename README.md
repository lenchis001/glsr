# glsr

**glsr** is a software OpenGL ES-like rendering library (GL Software Renderer), originally developed as part of
[TEngine](https://github.com/lenchis001/TEngine) and extracted into its own standalone repository.

## Building

```sh
cmake -S . -B build
cmake --build build
```

## Running tests

Tests are built automatically when glsr is configured as the top-level project. Run them with CTest:

```sh
ctest --test-dir build
```

## Using glsr as a dependency

When consumed from another CMake project (e.g. via `FetchContent`), tests are not built by default.
Set `GLSR_BUILD_TESTS` explicitly if you need to override this behavior.

```cmake
include(FetchContent)

FetchContent_Declare(
    glsr
    GIT_REPOSITORY https://github.com/<owner>/glsr.git
    GIT_TAG        main
)

FetchContent_MakeAvailable(glsr)

target_link_libraries(my_target PRIVATE glsr)
```
