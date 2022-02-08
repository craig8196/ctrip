
cmake_minimum_required(VERSION 3.16)
enable_testing()
project(ctrip VERSION 0.0.1 DESCRIPTION "The River Protocol")

include(GNUInstallDirs)

if(CODE_COVERAGE)
    set(CMAKE_BUILD_TYPE DEBUG)
    include(cmake-scripts/code-coverage.cmake)
endif()

add_subdirectory(test)
add_subdirectory(hitime)

file(GLOB CTRIP_SOURCES src/*.c)

#set(SOURCES src/trip.c src/trip_extra.c hitime/src/hitime.c hitime/src/hitime_extra.c)

add_library(ctrip STATIC ${CTRIP_SOURCES})
set_target_properties(ctrip PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(ctrip PROPERTIES PUBLIC_HEADER include/trip.h)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_C_FLAGS_DEBUG "-O0")

target_link_libraries(ctrip PRIVATE hitime)

target_include_directories(ctrip PRIVATE hitime/include)
target_include_directories(ctrip PRIVATE src)

target_include_directories(ctrip PUBLIC include)

configure_file(ctrip.pc.in ctrip.pc @ONLY)

if(CODE_COVERAGE)
    target_code_coverage(ctrip)
endif()

if(BUILD_DOCUMENTATION)
    set(MYPROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
    set(MYPROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set(PROJECT_README_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)
    include(cmake-scripts/doxygen.cmake)
    build_docs(ADD_TO_DOC ctrip)
endif()

install(TARGETS ctrip
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/ctrip.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)


