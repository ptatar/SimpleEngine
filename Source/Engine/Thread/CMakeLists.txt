set(THREAD_LIB_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(THREAD_LIB_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)
set(LIBRARY_INC_DIR ${THREAD_LIB_INC_DIR} PARENT_SCOPE)


set(THREAD_LIB_SRC
    ${THREAD_LIB_INC_DIR}/IJob.hpp
    ${THREAD_LIB_INC_DIR}/ThreadManager.hpp
    ${THREAD_LIB_SRC_DIR}/ThreadManager.cpp
)

add_library(THREAD_LIB STATIC ${THREAD_LIB_SRC})

target_link_libraries(THREAD_LIB COMMON_LIB
                                 Threads::Threads) #not my var name

target_include_directories(THREAD_LIB PUBLIC ${THREAD_LIB_INC_DIR})
