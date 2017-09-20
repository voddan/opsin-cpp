add_subdirectory(base_c)
add_subdirectory(base_cpp)

add_library(utils)

target_sources(utils PRIVATE
        RectangularVectors.h
        stringbuilder.h
        stringhelper.h
        collectionhelper.hpp
        Comparator.hpp)

target_link_libraries(utils base_c base_cpp)

target_include_directories(utils INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/../
        $<TARGET_PROPERTY:base_c,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:base_cpp,INTERFACE_INCLUDE_DIRECTORIES>)

set_target_properties(utils PROPERTIES LINKER_LANGUAGE CXX)
