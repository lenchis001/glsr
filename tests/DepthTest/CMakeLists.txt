file(GLOB_RECURSE depth_test_sources_files
    "Sources/*.cpp"
    "Sources/*.c"
)

add_executable(DepthTest
    ${depth_test_sources_files}
)

# 
target_compile_definitions(DepthTest PRIVATE TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Dependencies

# GLSR
target_link_libraries(DepthTest PRIVATE
    glsr
)

# Snapshot Test Framework
target_link_libraries(DepthTest PRIVATE
    snapshot_test_framework
)

add_test(NAME DepthTest.UnitTests COMMAND DepthTest)
