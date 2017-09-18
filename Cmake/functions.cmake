function(include_library path include_path)
    add_subdirectory(${path})
    set(${include_path} ${LIBRARY_INC_DIR} PARENT_SCOPE)
endfunction()
