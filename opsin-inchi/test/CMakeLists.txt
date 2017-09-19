add_executable(opsin-inchi-test
        src/InchiOutputTest.cpp
        src/InchiOutputTest.h
        src/NomenclatureIntegrationTest.cpp
        src/NomenclatureIntegrationTest.h)

target_link_libraries(opsin-inchi-test opsin-inchi)
target_include_directories(opsin-inchi-test PRIVATE $<TARGET_PROPERTY:opsin-core,INTERFACE_INCLUDE_DIRECTORIES>)