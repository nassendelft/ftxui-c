cmake_minimum_required(VERSION 3.12)
project(ftxui_c_binding)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Kotlin/Native links the static libraries (this binding + the FTXUI archives
# shipped in releases) into PIE executables and shared objects, so every
# object must be position independent.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add FTXUI as a subdirectory
add_subdirectory(vendor/ftxui ftxui_build)

# Create the shared library for the C binding
add_library(ftxui_c_binding_shared SHARED ftxui_c_wrapper.cpp)
target_link_libraries(ftxui_c_binding_shared PRIVATE ftxui::ftxui)
target_include_directories(ftxui_c_binding_shared PUBLIC .)
set_target_properties(ftxui_c_binding_shared PROPERTIES OUTPUT_NAME "ftxui_c_binding")

# Create the static library for the C binding
add_library(ftxui_c_binding_static STATIC ftxui_c_wrapper.cpp compat.c)
target_link_libraries(ftxui_c_binding_static PRIVATE ftxui::ftxui)
target_include_directories(ftxui_c_binding_static PUBLIC .)
set_target_properties(ftxui_c_binding_static PROPERTIES OUTPUT_NAME "ftxui_c_binding")

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

# Install rules. FTXUI installs its own libraries and headers via its
# FTXUI_ENABLE_INSTALL option (ON by default), which static consumers
# of ftxui_c_binding need at link time.
include(GNUInstallDirs)

install(TARGETS ftxui_c_binding_shared ftxui_c_binding_static
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES ftxui_c_api.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
