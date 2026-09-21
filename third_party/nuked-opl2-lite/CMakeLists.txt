# SPDX-License-Identifier: LGPL-2.1-or-later
# Restunts build integration, 2026-09-21.
cmake_minimum_required(VERSION 3.25)
project(NukedOPL2Lite VERSION 0.9.0 LANGUAGES C)

option(NUKED_OPL2_SSE2 "Allow SSE2 in 32-bit x86 builds" ON)
if(CMAKE_SIZEOF_VOID_P EQUAL 4 AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    if(NUKED_OPL2_SSE2)
        add_compile_options(-msse2)
    else()
        add_compile_options(-mno-sse -mno-sse2 -mno-avx -mfpmath=387)
    endif()
endif()
if(WIN32 AND MINGW)
    add_compile_definitions(_WIN32_WINNT=0x0601 WINVER=0x0601)
    add_link_options(-static-libgcc)
endif()

add_library(nuked_opl2 SHARED opl2.c)
target_include_directories(nuked_opl2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(nuked_opl2 PUBLIC NUKED_OPL2_SHARED PRIVATE NUKED_OPL2_BUILD)
set_target_properties(nuked_opl2 PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED ON
    OUTPUT_NAME nuked-opl2
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
if(WIN32)
    set_target_properties(nuked_opl2 PROPERTIES PREFIX "")
endif()

if(PROJECT_IS_TOP_LEVEL)
    install(TARGETS nuked_opl2 RUNTIME DESTINATION bin LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
    install(FILES opl2.h DESTINATION include/nuked-opl2-lite)
    install(FILES LICENSE README.md DESTINATION share/licenses/nuked-opl2-lite)
    install(FILES opl2.c opl2.h CMakeLists.txt LICENSE README.md
        DESTINATION share/nuked-opl2-lite)
endif()
