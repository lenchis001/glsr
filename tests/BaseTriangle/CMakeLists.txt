file(GLOB_RECURSE base_triangle_sources_files
    "Sources/*.cpp"
    "Sources/*.c"
)

add_executable(BaseTriangle
    ${base_triangle_sources_files}
)

# 
target_compile_definitions(BaseTriangle PRIVATE TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Dependencies

# GLSR
target_link_libraries(BaseTriangle PRIVATE
    glsr
)

# Snapshot Test Framework
target_link_libraries(BaseTriangle PRIVATE
    snapshot_test_framework
)

add_test(NAME BaseTriangle.UnitTests COMMAND BaseTriangle)