set(SHADER_COOKER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(SHADER_COOKER_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)

set(SHADER_COOKER_SRC
    ${SHADER_COOKER_SRC_DIR}/main.cpp
    ${SHADER_COOKER_INC_DIR}/ArgumentParser.hpp
    ${SHADER_COOKER_INC_DIR}/ArgumentForwardIter.hpp
    ${SHADER_COOKER_INC_DIR}/ShaderCooker.hpp
    ${SHADER_COOKER_SRC_DIR}/ShaderCooker.cpp
    ${SHADER_COOKER_INC_DIR}/ShaderHeader.hpp
    )

add_executable(SHADER_COOKER_EXE ${SHADER_COOKER_SRC})

target_link_libraries(SHADER_COOKER_EXE COMMON
                                        shaderc
                                        spirv-cross-glsl)

target_include_directories(SHADER_COOKER_EXE PRIVATE ${COMMON_INC_DIR}
                                             PRIVATE ${SHADER_COOKER_INC_DIR}
                                             PRIVATE ${SHADERC_INC_DIR}
                                             PRIVATE ${SPIRV_CROSS_INC_DIR}
                                             )

set_target_properties(SHADER_COOKER_EXE
                      PROPERTIES OUTPUT_NAME "ShaderCooker")

#get_target_property(NAME_PREFIX SHADER_COOKER_EXE PREFIX)
#get_target_property(NAME_SUFFIX SHADER_COOKER_EXE SUFFIX)
get_target_property(NAME_STEM SHADER_COOKER_EXE OUTPUT_NAME)

set(FULLNAME "${NAME_STEM}")

set(COOK_SHADER_SCRIPT_SOURCE "\
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${FULLNAME} \
--inputPath ${ASSETS_INPUT_DIRECTORY}/Shaders \
--outputPath ${ASSETS_OUTPUT_DIRECTORY}/Shaders")

file(WRITE "${SCRIPTS_OUTPUT_DIRECTORY}/CookShaders.sh" ${COOK_SHADER_SCRIPT_SOURCE})
