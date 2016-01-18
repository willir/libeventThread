cmake_minimum_required(VERSION 2.8.10)
project(thread_utils)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if((CMAKE_BUILD_TYPE STREQUAL "Debug") OR (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
    add_definitions(-DENABLE_DEBUG=1)
endif()

find_package(Threads REQUIRED)
find_package(Boost 1.54.0 REQUIRED COMPONENTS system thread)

#LibEvent:
find_library(LibEvent_LIBS event)
find_library(LibEvent_pthreads_LIBS event_pthreads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

set (SOURCE_FILES
            libevent_utils.cpp
            debug_utils.cpp
            Handler.cpp
            Looper.cpp
            ThreadLooper.cpp main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE -fexceptions)

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} ${LibEvent_LIBS})
target_link_libraries(${PROJECT_NAME} ${LibEvent_pthreads_LIBS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
