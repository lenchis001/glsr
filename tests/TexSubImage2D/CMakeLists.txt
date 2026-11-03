file(GLOB_RECURSE tex_sub_image_2d_sources_files
    "Sources/*.cpp"
    "Sources/*.c"
)

add_executable(TexSubImage2D
    ${tex_sub_image_2d_sources_files}
)

target_compile_definitions(TexSubImage2D PRIVATE TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(TexSubImage2D PRIVATE
    glsr
)

# Snapshot Test Framework
target_link_libraries(TexSubImage2D PRIVATE
    snapshot_test_framework
)

add_test(NAME TexSubImage2D.UnitTests COMMAND TexSubImage2D)

include(FetchContent)

FetchContent_Declare(
    stb
    TLS_VERIFY OFF
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(stb)

target_include_directories(TexSubImage2D PRIVATE
    ${stb_SOURCE_DIR}
)