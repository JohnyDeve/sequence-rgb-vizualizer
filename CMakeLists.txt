cmake_minimum_required(VERSION 3.16)

project(srgbviz VERSION 1.0.0 LANGUAGES CXX)

# ---------------------------
# Compiler Settings
# ---------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------
# Output directories
# ---------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ---------------------------
# Options
# ---------------------------
option(SRGBVIZ_BUILD_EXAMPLES "Build examples" ON)
option(SRGBVIZ_BUILD_TESTS "Build tests" OFF)
option(SRGBVIZ_BUILD_TOOLS "Build tools" OFF)

# ---------------------------
# Library
# ---------------------------
add_library(srgbviz
    src/bytepart_scale.cpp
    src/console_writer.cpp
    src/grayscale.cpp
    src/ppm_writer_stream.cpp
    src/ppm_writer.cpp
    src/video_writer.cpp
)

add_library(srgbviz::srgbviz ALIAS srgbviz)

# PUBLIC include
target_include_directories(srgbviz
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Warnings (if the file exists)
include(cmake/compiler_warnings.cmake OPTIONAL)
if(COMMAND set_project_warnings)
    set_project_warnings(srgbviz)
endif()

# ---------------------------
# Examples
# ---------------------------
if(SRGBVIZ_BUILD_EXAMPLES)
    add_executable(srgbviz_example
        #examples/basic_example.cpp
        #examples/functor_source_example.cpp
        examples/advanced_example.cpp
    )

    target_link_libraries(srgbviz_example PRIVATE srgbviz)
endif()

# ---------------------------
# Tools (CLI)
# ---------------------------
if(SRGBVIZ_BUILD_TOOLS)
    add_executable(srgbviz_cli
        tools/srgbviz_cli.cpp
    )

    target_link_libraries(srgbviz_cli PRIVATE srgbviz)
endif()

# ---------------------------
# Tests
# ---------------------------
if(SRGBVIZ_BUILD_TESTS)
    enable_testing()

    add_executable(srgbviz_tests
        tests/test_visualizer.cpp
    )

    target_link_libraries(srgbviz_tests PRIVATE srgbviz)

    add_test(NAME srgbviz_tests COMMAND srgbviz_tests)
endif()


# ---------------------------
# Installation
# ---------------------------
# include(GNUInstallDirs)

# install(TARGETS srgbviz
#     EXPORT srgbvizTargets
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# install(DIRECTORY include/
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# install(EXPORT srgbvizTargets
#     FILE srgbvizTargets.cmake
#     NAMESPACE srgbviz::
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/srgbviz
# )

# # ---------------------------
# # Config file (find_package)
# # ---------------------------
# include(CMakePackageConfigHelpers)

# write_basic_package_version_file(
#     "${CMAKE_CURRENT_BINARY_DIR}/srgbvizConfigVersion.cmake"
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY SameMajorVersion
# )

# configure_package_config_file(
#     cmake/srgbvizConfig.cmake.in
#     "${CMAKE_CURRENT_BINARY_DIR}/srgbvizConfig.cmake"
#     INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/srgbviz
# )

# install(FILES
#     "${CMAKE_CURRENT_BINARY_DIR}/srgbvizConfig.cmake"
#     "${CMAKE_CURRENT_BINARY_DIR}/srgbvizConfigVersion.cmake"
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/srgbviz
# )