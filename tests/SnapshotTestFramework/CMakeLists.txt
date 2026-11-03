file(GLOB_RECURSE snapshot_test_framework_sources_files
    "Sources/*.cpp"
    "Sources/*.c"
)

add_library(snapshot_test_framework STATIC ${snapshot_test_framework_sources_files})

target_include_directories(snapshot_test_framework PUBLIC "Sources")

include(FetchContent)

FetchContent_Declare(
    stb
    TLS_VERIFY OFF
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(stb)
message(STATUS "stb configuration completed.")

target_include_directories(snapshot_test_framework PRIVATE ${stb_SOURCE_DIR})