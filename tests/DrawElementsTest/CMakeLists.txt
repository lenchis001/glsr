file(GLOB_RECURSE draw_elements_test_sources_files
    "Sources/*.cpp"
    "Sources/*.c"
)

add_executable(DrawElementsTest
    ${draw_elements_test_sources_files}
)

# 
target_compile_definitions(DrawElementsTest PRIVATE TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Dependencies

# GLSR
target_link_libraries(DrawElementsTest PRIVATE
    glsr
)

# Snapshot Test Framework
target_link_libraries(DrawElementsTest PRIVATE
    snapshot_test_framework
)

add_test(NAME DrawElementsTest.UnitTests COMMAND DrawElementsTest)
