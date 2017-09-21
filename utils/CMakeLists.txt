add_subdirectory(base_c)
add_subdirectory(base_cpp)

add_library(utils)

target_sources(utils PRIVATE
        RectangularVectors.h
        stringbuilder.h
        stringhelper.h
        collectionhelper.hpp
        Comparator.hpp)

target_link_libraries(utils INTERFACE base_c base_cpp)

target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../)

set_target_properties(utils PROPERTIES LINKER_LANGUAGE CXX)
