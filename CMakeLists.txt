cmake_minimum_required(VERSION 3.8)
include("HunterGate.cmake")
HunterGate(URL "https://github.com/ruslo/hunter/archive/v0.16.15.tar.gz" SHA1 "6974c2150fc0d3b09de3ad1efcbf15d360647ffa")

project(opsin_cpp)
set(CMAKE_CXX_STANDARD 11)

hunter_add_package(Boost)
find_package(Boost 1.60 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})


add_subdirectory(utils)
add_subdirectory(automation-cpp)
add_subdirectory(opsin-core)
add_subdirectory(opsin-inchi)


add_executable(opsin_cpp main.cpp)

target_link_libraries(opsin_cpp opsin-core utils)
target_link_libraries(opsin_cpp LINK_PUBLIC ${Boost_LIBRARIES})

target_include_directories(opsin_cpp PRIVATE
        $<TARGET_PROPERTY:opsin-core,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:utils,INTERFACE_INCLUDE_DIRECTORIES>)