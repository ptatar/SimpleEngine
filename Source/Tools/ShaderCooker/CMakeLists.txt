set(SHADER_COOKER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(SHADER_COOKER_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)

set(SHADER_COOKER_SRC
    ${SHADER_COOKER_SRC_DIR}/main.cpp
    ${SHADER_COOKER_INC_DIR}/ArgumentParser.hpp
    ${SHADER_COOKER_SRC_DIR}/ArgumentParser.cpp
    ${SHADER_COOKER_INC_DIR}/ShaderCooker.hpp
    ${SHADER_COOKER_SRC_DIR}/ShaderCooker.cpp
    )

add_executable(SHADER_COOKER_EXE ${SHADER_COOKER_SRC})

target_link_libraries(SHADER_COOKER_EXE COMMON)

target_include_directories(SHADER_COOKER_EXE PRIVATE ${COMMON_INC_DIR})

target_include_directories(SHADER_COOKER_EXE PRIVATE ${SHADER_COOKER_INC_DIR})
