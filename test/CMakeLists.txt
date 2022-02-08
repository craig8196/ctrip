
cmake_minimum_required(VERSION 3.16)
enable_testing()

project(ctriptest)

add_executable(basics basics.c)
set_source_files_properties(basics PROPERTIES COMPILE_FLAGS "-Og" )
target_include_directories(basics PRIVATE ../include)
target_link_libraries(basics PRIVATE ctrip)
if(CODE_COVERAGE)
    target_code_coverage(basics)
endif()
add_test(NAME basics COMMAND basics)

add_executable(recv recv.c)
target_include_directories(recv PRIVATE ../include)
target_link_libraries(recv PRIVATE ctrip)
if(CODE_COVERAGE)
    target_code_coverage(recv)
endif()
add_test(NAME recv COMMAND recv)

