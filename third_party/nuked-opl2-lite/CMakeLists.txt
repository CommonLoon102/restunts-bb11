# SPDX-License-Identifier: LGPL-2.1-or-later
# Restunts build integration, 2026-09-21; WebAssembly and macOS support, 2026-09-29.
cmake_minimum_required(VERSION 3.25)
project(NukedOPL2Lite VERSION 0.9.0 LANGUAGES C)

option(NUKED_OPL2_SSE2 "Allow SSE2 in 32-bit x86 builds" ON)
# Keep the standalone replacement build valid on ARM32 and x86 multilib hosts.
include(CheckSymbolExists)
check_symbol_exists(__i386__ "" NUKED_OPL2_TARGET_X86)
if(NOT EMSCRIPTEN AND NUKED_OPL2_TARGET_X86 AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    if(NUKED_OPL2_SSE2)
        add_compile_options(-msse2)
    else()
        add_compile_options(-mno-sse -mno-sse2 -mno-avx -mfpmath=387)
    endif()
endif()
# The core uses no Windows API; its C runtime must remain bundled without
# requiring a newer Windows version than the parent game.
if(WIN32 AND MINGW)
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        add_link_options(-static-libgcc)
    else()
        add_link_options(-static)
    endif()
endif()

if(EMSCRIPTEN)
    add_library(nuked_opl2 STATIC opl2.c)
else()
    add_library(nuked_opl2 SHARED opl2.c)
    target_compile_definitions(nuked_opl2 PUBLIC NUKED_OPL2_SHARED PRIVATE NUKED_OPL2_BUILD)
endif()
target_include_directories(nuked_opl2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(nuked_opl2 PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED ON
    OUTPUT_NAME nuked-opl2
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
if(WIN32)
    set_target_properties(nuked_opl2 PROPERTIES PREFIX "")
elseif(APPLE)
    # Preserve the same relocatable identity for standalone replacement builds.
    set_target_properties(nuked_opl2 PROPERTIES MACOSX_RPATH ON INSTALL_NAME_DIR "@rpath")
endif()

if(PROJECT_IS_TOP_LEVEL)
    install(TARGETS nuked_opl2 RUNTIME DESTINATION bin LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
    install(FILES opl2.h DESTINATION include/nuked-opl2-lite)
    install(FILES LICENSE README.md DESTINATION share/licenses/nuked-opl2-lite)
    install(FILES opl2.c opl2.h CMakeLists.txt LICENSE README.md
        DESTINATION share/nuked-opl2-lite)
endif()
