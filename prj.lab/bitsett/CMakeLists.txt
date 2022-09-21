add_library(bitsett bitsett.h bitsett.cpp)

target_link_libraries(bitsett bitseti)

add_executable(bitsett_test bitsett_test.cpp)
target_link_libraries(bitsett_test bitsett)
