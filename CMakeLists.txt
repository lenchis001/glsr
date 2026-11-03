# CMakeList.txt : Top-level CMake project file for the standalone glsr library.
#
cmake_minimum_required (VERSION 3.20)

project ("glsr")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POLICY_VERSION_MINIMUM 3.20)

# Only build tests when glsr is the top-level project, e.g. not when it is
# pulled in as a dependency (FetchContent/add_subdirectory) by another project.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(GLSR_IS_TOP_LEVEL TRUE)
else()
  set(GLSR_IS_TOP_LEVEL FALSE)
endif()

option(GLSR_BUILD_TESTS "Build the glsr tests" ${GLSR_IS_TOP_LEVEL})

file(GLOB_RECURSE glsr_sources_files
    "Sources/*.cpp"
    "Sources/*.c"
)

add_library(glsr STATIC ${glsr_sources_files})

target_include_directories(glsr PUBLIC "Sources")

# --------------------- Dependencies ---------------------

# Download and configure glslang
include(FetchContent)

FetchContent_Declare(
    glslang
    GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
    GIT_TAG        main
    GIT_SHALLOW    TRUE
)

set(ENABLE_OPT OFF CACHE BOOL "" FORCE)
set(ENABLE_HLSL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glslang)
message(STATUS "glslang configuration completed.")

target_include_directories(glsr PRIVATE ${glslang_SOURCE_DIR})
target_link_libraries(glsr PRIVATE glslang glslang-default-resource-limits)

# Download and configure SPIRV-VM
include(FetchContent)

FetchContent_Declare(
    SPIRVVM
    GIT_REPOSITORY https://github.com/dfranx/SPIRV-VM.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(SPIRVVM)
message(STATUS "SPIRV-VM configuration completed.")

target_include_directories(glsr PRIVATE ${SPIRVVM_SOURCE_DIR}/inc)
target_link_libraries(glsr PRIVATE SPIRVVM)

# --------------------- Tests ---------------------

if(GLSR_BUILD_TESTS)
  find_package(Python3 REQUIRED COMPONENTS Interpreter)

  # Bake test Resources/ folders into generated C-arrays consumed by the tests.
  execute_process(
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/resources_baker.py" "${CMAKE_CURRENT_SOURCE_DIR}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RESULT_VARIABLE RESOURCES_BAKER_RESULT
  )

  if(NOT RESOURCES_BAKER_RESULT EQUAL 0)
    message(FATAL_ERROR "resources_baker.py failed with exit code ${RESOURCES_BAKER_RESULT}")
  endif()

  include(CTest)
  enable_testing()

  add_subdirectory(tests)
endif()
