file(GLOB_RECURSE textured_cube_sources_files
    "Sources/*.cpp"
    "Sources/*.c"
)

add_executable(TexturedCube
    ${textured_cube_sources_files}
)

# 
target_compile_definitions(TexturedCube PRIVATE TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Dependencies

# GLSR
target_link_libraries(TexturedCube PRIVATE
    glsr
)

# Snapshot Test Framework
target_link_libraries(TexturedCube PRIVATE
    snapshot_test_framework
)

add_test(NAME TexturedCube.UnitTests COMMAND TexturedCube)

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

target_include_directories(TexturedCube PRIVATE ${stb_SOURCE_DIR})
