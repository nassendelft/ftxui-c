cmake_minimum_required(VERSION 3.13)
project(ftxui_c_binding VERSION 1.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Kotlin/Native links the static libraries (this binding + the FTXUI archives
# shipped in releases) into PIE executables and shared objects, so every
# object must be position independent.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Hide all C++ symbols by default; only the FTXUI_C_API-marked functions are
# exported. This keeps FTXUI's symbols out of the shared library so a host
# application linking its own FTXUI copy cannot clash with ours. compat.c is
# C and keeps default visibility on purpose: its weak __libc_single_threaded
# fallback must remain interposable by glibc at runtime.
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(FTXUI_C_BUILD_TESTS_DEFAULT ON)
else()
    set(FTXUI_C_BUILD_TESTS_DEFAULT OFF)
endif()
option(FTXUI_C_BUILD_TESTS "Build the ftxui-c test executables" ${FTXUI_C_BUILD_TESTS_DEFAULT})

# Add FTXUI as a subdirectory
add_subdirectory(vendor/ftxui ftxui_build)

# Create the shared library for the C binding
add_library(ftxui_c_binding_shared SHARED ftxui_c_wrapper.cpp)
target_link_libraries(ftxui_c_binding_shared PRIVATE ftxui::ftxui)
target_include_directories(ftxui_c_binding_shared PUBLIC .)
target_compile_definitions(ftxui_c_binding_shared PRIVATE FTXUI_C_BUILD_SHARED)
set_target_properties(ftxui_c_binding_shared PROPERTIES
    OUTPUT_NAME "ftxui_c_binding"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})

# Keep symbols pulled in from the FTXUI archives (and vague-linkage template
# instantiations) out of the shared library's dynamic symbol table; only the
# ftxui_* C API is exported.
if(APPLE)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/exported_symbols.txt "_ftxui_*\n")
    target_link_options(ftxui_c_binding_shared PRIVATE
        -Wl,-exported_symbols_list,${CMAKE_CURRENT_BINARY_DIR}/exported_symbols.txt)
elseif(UNIX)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/version_script.map
        "{ global: ftxui_*; local: *; };\n")
    target_link_options(ftxui_c_binding_shared PRIVATE
        -Wl,--version-script=${CMAKE_CURRENT_BINARY_DIR}/version_script.map)
endif()

# Create the static library for the C binding
add_library(ftxui_c_binding_static STATIC ftxui_c_wrapper.cpp compat.c)
target_link_libraries(ftxui_c_binding_static PRIVATE ftxui::ftxui)
target_include_directories(ftxui_c_binding_static PUBLIC .)
set_target_properties(ftxui_c_binding_static PROPERTIES OUTPUT_NAME "ftxui_c_binding")

if(FTXUI_C_BUILD_TESTS)
    # Add a test executable
    add_executable(test_c test_c.c)
    target_link_libraries(test_c PRIVATE ftxui_c_binding_shared)
    target_include_directories(test_c PRIVATE .)

    # App API tests
    add_executable(test_app_api test_app_api.c)
    target_link_libraries(test_app_api PRIVATE ftxui_c_binding_shared)
    target_include_directories(test_app_api PRIVATE .)

    # Destructor tests
    add_executable(test_destructor test_destructor.c)
    target_link_libraries(test_destructor PRIVATE ftxui_c_binding_shared)
    target_include_directories(test_destructor PRIVATE .)

    enable_testing()
    add_test(NAME test_app_api COMMAND test_app_api)
    add_test(NAME test_destructor COMMAND test_destructor)
endif()

# Install rules. FTXUI installs its own libraries and headers via its
# FTXUI_ENABLE_INSTALL option (ON by default), which static consumers
# of ftxui_c_binding need at link time.
include(GNUInstallDirs)

install(TARGETS ftxui_c_binding_shared ftxui_c_binding_static
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES ftxui_c_api.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ftxui_c.def DESTINATION ${CMAKE_INSTALL_DATADIR}/ftxui-c)
