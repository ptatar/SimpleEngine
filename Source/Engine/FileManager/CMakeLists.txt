set(FILE_MANAGER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(FILE_MANAGER_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)
set(LIBRARY_INC_DIR ${FILE_MANAGER_INC_DIR} PARENT_SCOPE)

set(FILE_MANAGER_SRC
    ${FILE_MANAGER_INC_DIR}/FileManager.hpp
    ${FILE_MANAGER_SRC_DIR}/FileManager.cpp
)

add_library(FILE_MANAGER STATIC ${FILE_MANAGER_SRC})

#target_link_libraries(FILE_MANAGER COMMON)

target_include_directories(FILE_MANAGER PUBLIC ${FILE_MANAGER_INC_DIR}
                                        PRIVATE ${COMMON_INC_DIR})
