# Small C-only WebM playback: Mozilla nestegg demuxing and libvpx VP8 decoding.
# Configure scripts, Perl, assemblers, C++ and host tools are not needed to build.
include(FetchContent)
FetchContent_Declare(restunts_vpx
    URL https://github.com/webmproject/libvpx/archive/refs/tags/v1.17.0.tar.gz
    URL_HASH SHA256=1020f184046187baa2985dbde38e0691f49c44088bca7a1842b0236c6081dc0a
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_Declare(restunts_nestegg
    URL https://github.com/mozilla/nestegg/archive/767aab25013acefbdcc6d68a2a7a2c9081303e3f.tar.gz
    URL_HASH SHA256=bd597e86e5a7c00171aa56d914501e3f597ea46dbc7d432324e97da94a8a4b5e
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(restunts_vpx restunts_nestegg)

set(restunts_vpx_config_directory "${CMAKE_SOURCE_DIR}/third_party/libvpx-config")
set(restunts_vpx_generated_directory "${CMAKE_CURRENT_BINARY_DIR}/libvpx-config")
set(restunts_vpx_big_endian 0)
if(CMAKE_C_BYTE_ORDER STREQUAL "BIG_ENDIAN")
    set(restunts_vpx_big_endian 1)
endif()
configure_file("${restunts_vpx_config_directory}/vpx_config.h.in"
    "${restunts_vpx_generated_directory}/vpx_config.h" @ONLY)

# This is the source list of the pinned upstream generic-gnu, VP8-only decoder
# configuration documented in third_party/libvpx-config/README.md.
set(restunts_vpx_sources
    vp8/common/alloccommon.c
    vp8/common/blockd.c
    vp8/common/dequantize.c
    vp8/common/entropy.c
    vp8/common/entropymode.c
    vp8/common/entropymv.c
    vp8/common/extend.c
    vp8/common/filter.c
    vp8/common/findnearmv.c
    vp8/common/generic/systemdependent.c
    vp8/common/idct_blk.c
    vp8/common/idctllm.c
    vp8/common/loopfilter_filters.c
    vp8/common/mbpitch.c
    vp8/common/modecont.c
    vp8/common/quant_common.c
    vp8/common/reconinter.c
    vp8/common/reconintra.c
    vp8/common/reconintra4x4.c
    vp8/common/rtcd.c
    vp8/common/setupintrarecon.c
    vp8/common/swapyv12buffer.c
    vp8/common/treecoder.c
    vp8/common/vp8_loopfilter.c
    vp8/decoder/dboolhuff.c
    vp8/decoder/decodeframe.c
    vp8/decoder/decodemv.c
    vp8/decoder/detokenize.c
    vp8/decoder/onyxd_if.c
    vp8/vp8_dx_iface.c
    vpx/src/vpx_codec.c
    vpx/src/vpx_decoder.c
    vpx/src/vpx_encoder.c
    vpx/src/vpx_image.c
    vpx_dsp/bitreader.c
    vpx_dsp/bitreader_buffer.c
    vpx_dsp/intrapred.c
    vpx_dsp/prob.c
    vpx_dsp/skin_detection.c
    vpx_dsp/vpx_dsp_rtcd.c
    vpx_mem/vpx_mem.c
    vpx_scale/generic/gen_scalers.c
    vpx_scale/generic/vpx_scale.c
    vpx_scale/generic/yv12config.c
    vpx_scale/generic/yv12extend.c
    vpx_scale/vpx_scale_rtcd.c
    vpx_util/vpx_thread.c
    vpx_util/vpx_write_yuv_frame.c
)
list(TRANSFORM restunts_vpx_sources PREPEND "${restunts_vpx_SOURCE_DIR}/")
add_library(restunts_webm STATIC ${restunts_vpx_sources}
    "${restunts_vpx_config_directory}/vpx_config.c"
    "${restunts_nestegg_SOURCE_DIR}/src/nestegg.c")
# libvpx 1.17 requires C11; the game remains C99, including the decoder API caller.
set_target_properties(restunts_webm PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON)
target_include_directories(restunts_webm PRIVATE
    "${restunts_vpx_generated_directory}" "${restunts_vpx_config_directory}")
target_include_directories(restunts_webm SYSTEM PUBLIC
    "${restunts_vpx_SOURCE_DIR}" "${restunts_nestegg_SOURCE_DIR}/include")

foreach(component Runtime Tests)
    set(exclude_from_default)
    if(component STREQUAL "Tests")
        set(exclude_from_default EXCLUDE_FROM_ALL)
    endif()
    install(FILES "${restunts_vpx_SOURCE_DIR}/LICENSE"
        DESTINATION share/licenses/restunts RENAME libvpx-LICENSE
        COMPONENT ${component} ${exclude_from_default})
    install(FILES "${restunts_vpx_SOURCE_DIR}/PATENTS"
        DESTINATION share/licenses/restunts RENAME libvpx-PATENTS
        COMPONENT ${component} ${exclude_from_default})
    install(FILES "${restunts_nestegg_SOURCE_DIR}/LICENSE"
        DESTINATION share/licenses/restunts RENAME nestegg-LICENSE
        COMPONENT ${component} ${exclude_from_default})
endforeach()
