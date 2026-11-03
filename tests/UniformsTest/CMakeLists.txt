file(GLOB_RECURSE uniforms_test_sources_files
    "Sources/*.cpp"
    "Sources/*.c"
)

add_executable(UniformsTest
    ${uniforms_test_sources_files}
)

# 
target_compile_definitions(UniformsTest PRIVATE TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Dependencies

# GLSR
target_link_libraries(UniformsTest PRIVATE
    glsr
)

# Snapshot Test Framework
target_link_libraries(UniformsTest PRIVATE
    snapshot_test_framework
)

add_test(NAME UniformsTest.UnitTests COMMAND UniformsTest)
