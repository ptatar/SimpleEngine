set(COMMON_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(COMMON_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)
set(LIBRARY_INC_DIR ${COMMON_INC_DIR} PARENT_SCOPE)

set(COMMON_SRC
    ${COMMON_INC_DIR}/Types.hpp
    ${COMMON_INC_DIR}/Utility.hpp
    ${COMMON_INC_DIR}/Logger.hpp
    ${COMMON_INC_DIR}/DArray.hpp
    ${COMMON_INC_DIR}/Assert.hpp
    ${COMMON_INC_DIR}/Path.hpp
    ${COMMON_INC_DIR}/IFile.hpp
    ${COMMON_INC_DIR}/File.hpp
    ${COMMON_INC_DIR}/FileUtility.hpp
    ${COMMON_INC_DIR}/Config.hpp
    ${COMMON_INC_DIR}/ConfigImpl.hpp
    ${COMMON_SRC_DIR}/DArray.cpp
    ${COMMON_SRC_DIR}/Utility.cpp
    ${COMMON_SRC_DIR}/Path.cpp
    ${COMMON_SRC_DIR}/Logger.cpp
    ${COMMON_SRC_DIR}/IFile.cpp
    ${COMMON_SRC_DIR}/File.cpp
    ${COMMON_SRC_DIR}/FileUtility.cpp
    ${COMMON_SRC_DIR}/Config.cpp
    ${COMMON_SRC_DIR}/ConfigImpl.cpp
    ${COMMON_INC_DIR}/Serializable.hpp
)

add_library(COMMON STATIC ${COMMON_SRC})

target_link_libraries(COMMON "-lstdc++fs")

target_include_directories(COMMON PUBLIC ${COMMON_INC_DIR}
                                  PRIVATE ${RAPIDJSON_INC_DIR}
                                  PUBLIC ${YAS_INC_DIR})
