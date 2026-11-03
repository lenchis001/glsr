file(GLOB_RECURSE buffer_sub_data_test_sources_files
    "Sources/*.cpp"
    "Sources/*.c"
)

add_executable(BufferSubDataTest
    ${buffer_sub_data_test_sources_files}
)

# 
target_compile_definitions(BufferSubDataTest PRIVATE TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Dependencies

# GLSR
target_link_libraries(BufferSubDataTest PRIVATE
    glsr
)

# Snapshot Test Framework
target_link_libraries(BufferSubDataTest PRIVATE
    snapshot_test_framework
)

add_test(NAME BufferSubDataTest.UnitTests COMMAND BufferSubDataTest)
