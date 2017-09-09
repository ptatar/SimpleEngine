function(include_library path include_path)
    add_subdirectory(${path})
    set(${include_path} ${INCLUDE_DIR})
endfunction()
