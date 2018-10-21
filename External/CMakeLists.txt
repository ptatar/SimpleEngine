set(RAPIDJSON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson)
set(GLSLANG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glslang)

add_subdirectory(${RAPIDJSON_DIR})
add_subdirectory(${GLSLANG_DIR})

set(GLSLLANG_LIB_INC_DIR ${GLSLANG_DIR}/glslang/Include)
