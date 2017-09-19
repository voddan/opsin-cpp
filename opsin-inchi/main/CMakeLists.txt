add_library(opsin-inchi
        opsin-inchi/InchiPruner.cpp
        opsin-inchi/InchiPruner.h
        opsin-inchi/NameToInchi.cpp
        opsin-inchi/NameToInchi.h)

target_include_directories(opsin-inchi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(opsin-inchi PROPERTIES LINKER_LANGUAGE CXX)
