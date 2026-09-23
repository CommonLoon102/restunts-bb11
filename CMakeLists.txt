cmake_minimum_required(VERSION 3.25)
project(restunts VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
# SDL's DOS/POSIX backends need the platform declarations hidden by strict ANSI.
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Both build-tree and installed binaries must accept a user-rebuilt OPL library.
set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

option(RESTUNTS_SYSTEM_SDL "Use an installed SDL3 instead of the pinned source" OFF)
option(RESTUNTS_BUILD_TESTS "Build native platform regressions" ON)
if(CMAKE_SYSTEM_NAME STREQUAL "DOS")
    set(restunts_sse2_default OFF)
else()
    set(restunts_sse2_default ON)
endif()
option(RESTUNTS_SSE2 "Allow SSE2 in 32-bit x86 builds (including bundled SDL)" ${restunts_sse2_default})

if(CMAKE_SIZEOF_VOID_P EQUAL 4 AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    if(RESTUNTS_SSE2)
        add_compile_options(-msse2)
    else()
        if(RESTUNTS_SYSTEM_SDL)
            message(FATAL_ERROR "RESTUNTS_SSE2=OFF requires bundled SDL to control its instruction set")
        endif()
        add_compile_options(-mno-sse -mno-sse2 -mno-avx -mfpmath=387)
        set(SDL_ASSEMBLY OFF CACHE BOOL "No SIMD instructions" FORCE)
        foreach(feature MMX SSE SSE2 SSE3 SSE4_1 SSE4_2 AVX AVX2 AVX512F)
            set(SDL_${feature} OFF CACHE BOOL "No SIMD instructions" FORCE)
        endforeach()
    endif()
elseif(NOT RESTUNTS_SSE2)
    message(FATAL_ERROR "RESTUNTS_SSE2=OFF is only supported on 32-bit x86; x64 requires SSE2")
endif()

if(WIN32)
    if(NOT MINGW)
        message(FATAL_ERROR "Windows builds require MinGW-w64")
    endif()
    add_compile_definitions(_WIN32_WINNT=0x0601 WINVER=0x0601)
    add_link_options(-static -static-libgcc -static-libstdc++)
endif()

if(RESTUNTS_SYSTEM_SDL)
    find_package(SDL3 3.4 REQUIRED CONFIG)
else()
    include(FetchContent)
    set(SDL_SHARED OFF CACHE BOOL "Bundle SDL statically")
    set(SDL_STATIC ON CACHE BOOL "Bundle SDL statically")
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "Disable SDL tests")
    set(SDL_TESTS OFF CACHE BOOL "Disable SDL tests")
    set(SDL_EXAMPLES OFF CACHE BOOL "Disable SDL examples")
    set(SDL_INSTALL OFF CACHE BOOL "Only install the game")
    # Upstream DOS support and the gameport/Sound Blaster IRQ fix used by
    # stuntsengine's DOS port. Pin all platforms to the same SDL implementation.
    FetchContent_Declare(SDL3
        URL https://github.com/libsdl-org/SDL/archive/015489c672f24feed28c2aa2cdd6176df95329f3.tar.gz
        URL_HASH SHA256=50c373115702974928c760f6e752aae33b69dcc51b4c2d909c35b30f41283a52
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    FetchContent_MakeAvailable(SDL3)
endif()

set(core src/restunts/c)
set(platform src/restunts/platform/sdl3)
set(common_sources
    fileio.c legacy.c resource.c math.c memmgr.c strlib.c numlib.c kevinrnd.c
    replinit.c gameinit.c gamestep.c simd.c carsetup.c trkutil.c trackres.c
    headless_data.c headless_trackdata.c state.c physics_grip.c opponent.c
    track_setup.c stateply.c physics_collision.c statecrs.c statecar.c trackobj.c
    owoot.c owoot_wheels.c owoot_road.c owoot_route.c replay.c stateio.c)
set(game_sources
    restunts.c fatal.c timing.c game_input.c ui_text.c ui_input.c ui_dialog.c
    intro.c highscore.c replay_record.c replay_viewer.c race.c ghost.c
    race_resources.c dashboard.c menu_common.c menu_main.c menu_options.c
    menu_track.c menu_track_editor.c menu_opponent.c menu_car.c audio_engine.c
    audio_resources.c audio_car.c audio_sequence.c shape3d.c shape3d_resources.c
    shape3d_car.c shape3d_prerender.c shape3d_lines.c shape2d.c shape2d_blit.c
    video_pages.c hires.c frame_prediction.c phantom_physics.c shape3d_hires.c shape2d_resources.c heapsort.c frame.c skybox.c skybox_hires.c frame_overlay.c
    intro_render.c track_preview.c full_data.c full_strings.c full_tables.c)
list(TRANSFORM common_sources PREPEND "${core}/")
list(TRANSFORM game_sources PREPEND "${core}/")
set(platform_common ${platform}/memory.c ${platform}/file.c ${platform}/runtime.c)
set(platform_game ${platform}/video.c ${platform}/input.c ${platform}/timer.c ${platform}/audio.c)

execute_process(COMMAND git -c safe.directory=${CMAKE_SOURCE_DIR} rev-parse HEAD
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE RESTUNTS_GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
if(NOT RESTUNTS_GIT_HASH)
    set(RESTUNTS_GIT_HASH "source-archive")
endif()

add_library(restunts_options INTERFACE)
target_compile_definitions(restunts_options INTERFACE RESTUNTS_SDL3
    RESTUNTS_GIT_HASH="${RESTUNTS_GIT_HASH}")
target_include_directories(restunts_options INTERFACE "${CMAKE_SOURCE_DIR}/${core}")
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(restunts_options INTERFACE -fsigned-char -fno-strict-aliasing
        -Wall -Wextra -Wno-pointer-sign -Wno-missing-field-initializers -Wno-missing-braces)
endif()
target_link_libraries(restunts_options INTERFACE SDL3::SDL3 m)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "DOS")
    set(NUKED_OPL2_SSE2 ${RESTUNTS_SSE2} CACHE BOOL "Match the game's x86 instruction set" FORCE)
    add_subdirectory(third_party/nuked-opl2-lite)
    add_library(restunts_opl ALIAS nuked_opl2)
endif()

add_library(restunts_game STATIC ${common_sources} ${game_sources} ${platform_common} ${platform_game})
target_compile_definitions(restunts_game PRIVATE RESTUNTS_FULL
    RESTUNTS_SKYBOX_DIRECTORY="${CMAKE_SOURCE_DIR}/assets/skyboxes")
target_link_libraries(restunts_game PUBLIC restunts_options)
if(TARGET restunts_opl)
    target_link_libraries(restunts_game PRIVATE restunts_opl)
endif()

add_executable(restunts ${platform}/main.c)
target_compile_definitions(restunts PRIVATE RESTUNTS_FULL)
target_link_libraries(restunts PRIVATE restunts_game)

add_executable(repldump ${common_sources} ${core}/headless.c ${platform_common}
    ${platform}/main.c src/restunts/repldump/repldump.c)
target_compile_definitions(repldump PRIVATE RESTUNTS_HEADLESS)
target_link_libraries(repldump PRIVATE restunts_options)

add_executable(pixldump ${platform}/main.c src/restunts/pixldump/pixldump.c
    src/restunts/pixldump/murmur3.c src/restunts/pixldump/legacy_context.c)
target_compile_definitions(pixldump PRIVATE RESTUNTS_PIXLDUMP)
target_link_libraries(pixldump PRIVATE restunts_game)

install(TARGETS restunts repldump pixldump RUNTIME DESTINATION bin COMPONENT Runtime)
install(FILES docs/sdl3.md DESTINATION share/docs/restunts COMPONENT Runtime)
set(skybox_themes desert tropical alpine city country)
set(skybox_images scen sce2 sce3 sce4)
foreach(theme RANGE 0 4)
    list(GET skybox_themes ${theme} theme_name)
    foreach(image RANGE 0 3)
        list(GET skybox_images ${image} image_name)
        install(FILES "assets/skyboxes/${theme_name}-${image_name}.png"
            DESTINATION bin/skyboxes RENAME "sky${theme}-${image}.png" COMPONENT Runtime)
    endforeach()
endforeach()
if(TARGET nuked_opl2)
    install(TARGETS nuked_opl2
        RUNTIME DESTINATION bin COMPONENT Runtime
        LIBRARY DESTINATION lib COMPONENT Runtime)
    configure_file(cmake/nuked-build-info.txt.in "${CMAKE_CURRENT_BINARY_DIR}/nuked-build-info.txt" @ONLY)
    foreach(component Runtime Tests)
        set(exclude_from_default)
        if(component STREQUAL "Tests")
            set(exclude_from_default EXCLUDE_FROM_ALL)
        endif()
        install(FILES third_party/nuked-opl2-lite/LICENSE
            DESTINATION share/licenses/restunts RENAME Nuked-OPL2-LICENSE
            COMPONENT ${component} ${exclude_from_default})
        install(FILES THIRD-PARTY-NOTICES.txt
            DESTINATION . COMPONENT ${component} ${exclude_from_default})
        install(FILES third_party/nuked-opl2-lite/opl2.c third_party/nuked-opl2-lite/opl2.h
            third_party/nuked-opl2-lite/CMakeLists.txt third_party/nuked-opl2-lite/LICENSE
            third_party/nuked-opl2-lite/README.md "${CMAKE_CURRENT_BINARY_DIR}/nuked-build-info.txt"
            DESTINATION share/restunts/nuked-opl2-lite COMPONENT ${component} ${exclude_from_default})
        # Preserve the parent build controls as source context, alongside the
        # standalone library build that recipients can use without the game.
        install(FILES CMakeLists.txt
            DESTINATION share/restunts/nuked-opl2-lite/build-context
            COMPONENT ${component} ${exclude_from_default})
        install(FILES cmake/nuked-build-info.txt.in
            DESTINATION share/restunts/nuked-opl2-lite/build-context/cmake
            COMPONENT ${component} ${exclude_from_default})
        install(FILES cmake/toolchains/linux-x86.cmake cmake/toolchains/mingw-x86.cmake
            cmake/toolchains/mingw-x64.cmake
            DESTINATION share/restunts/nuked-opl2-lite/build-context/cmake/toolchains
            COMPONENT ${component} ${exclude_from_default})
    endforeach()
endif()
if(NOT RESTUNTS_SYSTEM_SDL)
    foreach(component Runtime Tests)
        set(exclude_from_default)
        if(component STREQUAL "Tests")
            set(exclude_from_default EXCLUDE_FROM_ALL)
        endif()
        install(FILES "${SDL3_SOURCE_DIR}/LICENSE.txt"
            DESTINATION share/licenses/restunts RENAME SDL-LICENSE.txt
            COMPONENT ${component} ${exclude_from_default})
    endforeach()
endif()

if(RESTUNTS_BUILD_TESTS)
    enable_testing()
    add_executable(test-sdl3-platform src/restunts/tests/test-sdl3-platform.c ${platform_common})
    target_compile_definitions(test-sdl3-platform PRIVATE RESTUNTS_SDL3)
    target_compile_options(test-sdl3-platform PRIVATE -UNDEBUG)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/platform-test")
    add_test(NAME sdl3-platform COMMAND test-sdl3-platform)
    set_tests_properties(sdl3-platform PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/platform-test")
    if(UNIX AND NOT CMAKE_CROSSCOMPILING)
        add_test(NAME host-regressions COMMAND bash ${CMAKE_SOURCE_DIR}/src/restunts/tests/run-host-tests.sh)
        find_package(Python3 COMPONENTS Interpreter QUIET)
        if(Python3_Interpreter_FOUND)
            add_test(NAME opl-trace-renderer COMMAND "${Python3_EXECUTABLE}"
                "${CMAKE_SOURCE_DIR}/tools/scripts/test-render-opl-trace.py")
            add_test(NAME skybox-assets COMMAND "${Python3_EXECUTABLE}"
                "${CMAKE_SOURCE_DIR}/tools/scripts/check-skyboxes.py")
        endif()
    endif()
endif()

if(RESTUNTS_BUILD_TESTS AND TARGET restunts_opl)
    add_executable(test-sdl3-audio-trace src/restunts/tests/test-sdl3-audio-trace.c)
    target_compile_options(test-sdl3-audio-trace PRIVATE -UNDEBUG)
    add_test(NAME sdl3-audio-trace COMMAND test-sdl3-audio-trace)
    add_executable(test-sdl3-audio src/restunts/tests/test-sdl3-audio.c)
    target_link_libraries(test-sdl3-audio PRIVATE restunts_options restunts_opl)
    # Exercise real synthesis and SDL's dummy audio device, plus initialization
    # failure and batch-mode cleanup, without requiring host sound hardware.
    target_compile_options(test-sdl3-audio PRIVATE -UNDEBUG)
    add_test(NAME sdl3-audio COMMAND test-sdl3-audio "${CMAKE_SOURCE_DIR}/stunts/ADSKIDMS.VCE")
    # Regression runs must not overwrite a live capture inherited from the shell.
    set_tests_properties(sdl3-audio PROPERTIES ENVIRONMENT "RESTUNTS_AUDIO_TRACE=")
    add_executable(test-sdl3-audio-sequence src/restunts/tests/test-sdl3-audio-sequence.c)
    target_link_libraries(test-sdl3-audio-sequence PRIVATE restunts_game)
    target_compile_options(test-sdl3-audio-sequence PRIVATE -UNDEBUG)
    add_test(NAME sdl3-audio-sequence COMMAND test-sdl3-audio-sequence)
    set_tests_properties(sdl3-audio-sequence PROPERTIES ENVIRONMENT "RESTUNTS_AUDIO_TRACE=")
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        # This fixture includes the sequencer to reach its private marshaling
        # helper. ELF section GC isolates that helper from the rest of the game;
        # PE resolves unused references too, so the fixture is Linux-only.
        add_executable(test-sdl3-audio-definition src/restunts/tests/test-sdl3-audio-definition.c
            ${core}/resource.c ${platform}/memory.c ${platform}/runtime.c)
        target_link_libraries(test-sdl3-audio-definition PRIVATE restunts_options)
        target_compile_options(test-sdl3-audio-definition PRIVATE -UNDEBUG -ffunction-sections -fdata-sections)
        target_link_options(test-sdl3-audio-definition PRIVATE -Wl,--gc-sections)
        add_test(NAME sdl3-audio-definition COMMAND test-sdl3-audio-definition)
    endif()
endif()

if(RESTUNTS_BUILD_TESTS AND NOT CMAKE_SYSTEM_NAME STREQUAL "DOS")
    add_executable(test-sdl3-input src/restunts/tests/test-sdl3-input.c
        ${platform}/input.c ${platform}/video.c ${platform}/timer.c)
    target_link_libraries(test-sdl3-input PRIVATE restunts_options)
    target_compile_options(test-sdl3-input PRIVATE -UNDEBUG)
    add_test(NAME sdl3-input COMMAND test-sdl3-input)
endif()

if(RESTUNTS_BUILD_TESTS)
    add_executable(test-frame-prediction src/restunts/tests/test-frame-prediction.c ${core}/frame_prediction.c)
    target_link_libraries(test-frame-prediction PRIVATE restunts_options)
    target_compile_options(test-frame-prediction PRIVATE -UNDEBUG)
    add_test(NAME frame-prediction COMMAND test-frame-prediction)
    if(UNIX AND NOT CMAKE_CROSSCOMPILING)
        add_executable(test-sdl3-race-frames src/restunts/tests/test-race-frames.c
            ${core}/legacy.c ${core}/full_data.c ${core}/headless_data.c ${core}/frame_prediction.c)
        target_link_libraries(test-sdl3-race-frames PRIVATE restunts_options)
        target_compile_options(test-sdl3-race-frames PRIVATE -UNDEBUG -ffunction-sections -fdata-sections)
        target_link_options(test-sdl3-race-frames PRIVATE -Wl,--gc-sections)
        add_test(NAME sdl3-race-frames COMMAND test-sdl3-race-frames)
    endif()
    if(UNIX AND NOT CMAKE_CROSSCOMPILING AND Python3_Interpreter_FOUND)
        add_executable(test-render-replay ${platform}/main.c src/restunts/tests/test-render-replay.c)
        target_compile_definitions(test-render-replay PRIVATE RESTUNTS_PIXLDUMP)
        target_link_libraries(test-render-replay PRIVATE restunts_game)
        target_compile_options(test-render-replay PRIVATE -UNDEBUG)
        add_test(NAME render-replay COMMAND "${Python3_EXECUTABLE}"
            "${CMAKE_SOURCE_DIR}/tools/scripts/test-render-replay.py"
            "$<TARGET_FILE:test-render-replay>" "${CMAKE_SOURCE_DIR}/stunts")
    endif()
    add_executable(test-shape3d-hires src/restunts/tests/test-shape3d-hires.c)
    target_link_libraries(test-shape3d-hires PRIVATE restunts_game)
    target_compile_options(test-shape3d-hires PRIVATE -UNDEBUG)
    add_test(NAME shape3d-hires COMMAND test-shape3d-hires)
    add_executable(test-skybox-hires src/restunts/tests/test-skybox-hires.c
        ${core}/skybox_hires.c ${core}/hires.c ${platform}/memory.c)
    target_link_libraries(test-skybox-hires PRIVATE restunts_options)
    target_compile_options(test-skybox-hires PRIVATE -UNDEBUG)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/skybox-test")
    add_test(NAME skybox-hires COMMAND test-skybox-hires)
    set_tests_properties(skybox-hires PROPERTIES
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/skybox-test")
    add_executable(test-track-preview-skybox src/restunts/tests/test-track-preview-skybox.c)
    target_link_libraries(test-track-preview-skybox PRIVATE restunts_game)
    target_compile_options(test-track-preview-skybox PRIVATE -UNDEBUG)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/track-preview-test")
    add_test(NAME track-preview-skybox COMMAND test-track-preview-skybox)
    set_tests_properties(track-preview-skybox PROPERTIES
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/track-preview-test")
    add_executable(test-sdl3-hires src/restunts/tests/test-sdl3-hires.c ${core}/hires.c ${platform}/memory.c)
    target_link_libraries(test-sdl3-hires PRIVATE restunts_options)
    target_compile_options(test-sdl3-hires PRIVATE -UNDEBUG)
    add_test(NAME sdl3-hires COMMAND test-sdl3-hires)
    add_executable(test-sdl3-fileio src/restunts/tests/test-sdl3-fileio.c)
    target_link_libraries(test-sdl3-fileio PRIVATE restunts_game)
    target_compile_options(test-sdl3-fileio PRIVATE -UNDEBUG)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/fileio-test")
    add_test(NAME sdl3-fileio COMMAND test-sdl3-fileio)
    set_tests_properties(sdl3-fileio PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/fileio-test")
endif()

if(RESTUNTS_BUILD_TESTS AND TARGET nuked_opl2)
    install(TARGETS nuked_opl2
        RUNTIME DESTINATION bin COMPONENT Tests EXCLUDE_FROM_ALL
        LIBRARY DESTINATION lib COMPONENT Tests EXCLUDE_FROM_ALL)
    install(TARGETS test-shape3d-hires test-skybox-hires test-track-preview-skybox
        RUNTIME DESTINATION bin COMPONENT Tests EXCLUDE_FROM_ALL)
    foreach(test platform audio audio-trace audio-definition audio-sequence input fileio hires)
        if(TARGET test-sdl3-${test})
            install(TARGETS test-sdl3-${test}
                RUNTIME DESTINATION bin COMPONENT Tests EXCLUDE_FROM_ALL)
        endif()
    endforeach()
endif()
