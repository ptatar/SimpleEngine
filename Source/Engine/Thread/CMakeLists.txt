set(THREAD_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(THREAD_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)
set(LIBRARY_INC_DIR ${THREAD_INC_DIR} PARENT_SCOPE)


set(THREAD_SRC
    ${THREAD_INC_DIR}/IJob.hpp
    ${THREAD_INC_DIR}/ThreadManager.hpp
    ${THREAD_SRC_DIR}/ThreadManager.cpp
)

add_library(THREAD STATIC ${THREAD_SRC})

target_link_libraries(THREAD COMMON
                             Threads::Threads) #not my var name

target_include_directories(THREAD PUBLIC ${THREAD_INC_DIR})
