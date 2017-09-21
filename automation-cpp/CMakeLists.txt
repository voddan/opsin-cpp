add_library(automation
        automaton/Automaton.cpp
        automaton/Automaton.h
        automaton/AutomatonMatcher.cpp
        automaton/AutomatonMatcher.h
        automaton/AutomatonProvider.cpp
        automaton/AutomatonProvider.h
        automaton/BasicAutomata.cpp
        automaton/BasicAutomata.h
        automaton/BasicOperations.cpp
        automaton/BasicOperations.h
        automaton/Datatypes.cpp
        automaton/Datatypes.h
        automaton/DatatypesAutomatonProvider.cpp
        automaton/DatatypesAutomatonProvider.h
        automaton/MinimizationOperations.cpp
        automaton/MinimizationOperations.h
        automaton/RegExp.cpp
        automaton/RegExp.h
        automaton/RunAutomaton.cpp
        automaton/RunAutomaton.h
        automaton/ShuffleOperations.cpp
        automaton/ShuffleOperations.h
        automaton/SpecialOperations.cpp
        automaton/SpecialOperations.h
        automaton/State.cpp
        automaton/State.h
        automaton/StatePair.cpp
        automaton/StatePair.h
        automaton/StringUnionOperations.cpp
        automaton/StringUnionOperations.h
        automaton/Transition.cpp
        automaton/Transition.h
        automaton/TransitionComparator.cpp
        automaton/TransitionComparator.h)

target_include_directories(automation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(automation utils)

set_target_properties(automation PROPERTIES LINKER_LANGUAGE CXX)
