# Shared by the game build and its relocatable LGPL relink package.
function(restunts_configure_wasm target asset_root shell)
    # Legacy rendering uses large automatic arrays. Asyncify saves nested game
    # loops while the browser processes input, audio and display callbacks.
    set(restunts_stack_bytes 1048576)
    set(restunts_asyncify_stack_bytes 1048576)
    set_target_properties(${target} PROPERTIES SUFFIX ".html")
    target_link_options(${target} PRIVATE
        -sSINGLE_FILE=1
        -sENVIRONMENT=web
        -sASYNCIFY=1
        -sASYNCIFY_STACK_SIZE=${restunts_asyncify_stack_bytes}
        -sSTACK_SIZE=${restunts_stack_bytes}
        -sALLOW_MEMORY_GROWTH=1
        -sEXIT_RUNTIME=1
        "-sEXPORTED_RUNTIME_METHODS=['FS','callMain']"
        "SHELL:--shell-file \"${shell}\""
        "SHELL:--embed-file \"${asset_root}/assets@/assets\"")
    file(GLOB_RECURSE restunts_embedded_files CONFIGURE_DEPENDS
        "${asset_root}/assets/*")
    set_property(TARGET ${target} APPEND PROPERTY LINK_DEPENDS
        "${shell}" ${restunts_embedded_files})
endfunction()
