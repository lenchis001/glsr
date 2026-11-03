file(GLOB_RECURSE textured_triangle_sources_files
    "Sources/*.cpp"
    "Sources/*.c"
)

add_executable(TexturedTriangle
    ${textured_triangle_sources_files}
)

# 
target_compile_definitions(TexturedTriangle PRIVATE TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Dependencies

# GLSR
target_link_libraries(TexturedTriangle PRIVATE
    glsr
)

# Snapshot Test Framework
target_link_libraries(TexturedTriangle PRIVATE
    snapshot_test_framework
)

add_test(NAME TexturedTriangle.UnitTests COMMAND TexturedTriangle)

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

target_include_directories(TexturedTriangle PRIVATE ${stb_SOURCE_DIR})
