add_library(bitset bitset.h bitset.cpp)

target_link_libraries(bitset bitseti)

add_executable(bitset_test bitset_test.cpp)
target_link_libraries(bitset_test bitset)
