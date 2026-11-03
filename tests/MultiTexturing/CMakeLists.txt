file(GLOB_RECURSE multi_texture_sources_files
    "Sources/*.cpp"
    "Sources/*.c"
)

add_executable(MultiTexturing
    ${multi_texture_sources_files}
)

target_compile_definitions(MultiTexturing PRIVATE TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Dependencies

# GLSR
target_link_libraries(MultiTexturing PRIVATE
    glsr
)

# Snapshot Test Framework
target_link_libraries(MultiTexturing PRIVATE
    snapshot_test_framework
)

add_test(NAME MultiTexturing.UnitTests COMMAND MultiTexturing)

# Dependencies

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

target_include_directories(MultiTexturing PRIVATE ${stb_SOURCE_DIR})
