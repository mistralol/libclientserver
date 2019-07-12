cmake_minimum_required (VERSION 3.10.2)
enable_testing()

option(ENABLE_CPPCHECK "use cppcheck" OFF)
option(OPTIMIZE "Turn on compiler optimizations" ON)

project(libclientserver
    DESCRIPTION "Client Server lib for C++"
    VERSION "1.0.2"
    LANGUAGES CXX
)

add_definitions(-DPACKAGE_VERSION="${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

#Use ccache if its installed
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

#Use cppcheck if its installed
if(ENABLE_CPPCHECK)
    find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
    if (CMAKE_CXX_CPPCHECK)
        list(APPEND CMAKE_CXX_CPPCHECK
                "--enable=warning"
                "--inconclusive"
                "--inline-suppr"
        )
    endif()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CODECOV_OUTPUTFILE cmake_coverage.output )
    set(CODECOV_HTMLOUTPUTDIR coverage_results)

    if (CMAKE_COMPILER_IS_GNUCXX)
        find_program(CODECOV_GCOV gcov )
        find_program(CODECOV_LCOV lcov )
        find_program(CODECOV_GENHTML genhtml )
        add_definitions(-fprofile-arcs -ftest-coverage )
        link_libraries(gcov)
        set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} --coverage)
    endif (CMAKE_COMPILER_IS_GNUCXX )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

set(FLAGS "-ggdb -Wall -Wextra -Wundef -Wwrite-strings -Wpointer-arith -Wmissing-declarations -Wredundant-decls")
set(FLAGS "${FLAGS} -Wno-unused-parameter -Wno-missing-field-initializers -Wformat=2 -Wcast-align -Wformat-nonliteral -Wformat-security")
set(FLAGS "${FLAGS} -Wsign-compare -Wstrict-aliasing -Wshadow -Winline -Wpacked -Wmissing-format-attribute -Wmissing-noreturn -Winit-self")
set(FLAGS "${FLAGS} -Wmissing-include-dirs -Wunused-but-set-variable -Warray-bounds -Wreturn-type ")
set(FLAGS "${FLAGS} -Wno-switch -Wno-error=unused-parameter")
set(FLAGS "${FLAGS} -Wno-error=missing-field-initializers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS} -Wno-overloaded-virtual")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS}")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include(FindPkgConfig)
pkg_search_module(JSONCPP REQUIRED jsoncpp)
pkg_search_module(UUID REQUIRED uuid)
pkg_search_module(OPENSSL REQUIRED openssl)
pkg_search_module(CAP REQUIRED libcap)


add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libclientserver.pc.cmake libclientserver.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libclientserver.pc
    DESTINATION share/pkgconfig
)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "James Stevenson <james@stev.org>")
SET(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
INCLUDE(CPack)
