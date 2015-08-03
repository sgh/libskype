cmake_minimum_required(VERSION 2.8)
project( libskype )

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra")
	list(APPEND CMAKE_C_FLAGS "-Wall -Wextra")
endif()

find_package(PkgConfig)

## Find dbus-1
pkg_check_modules(PC_DBUS dbus-1 REQUIRED)
include_directories( ${PC_DBUS_INCLUDE_DIRS} )
link_libraries( ${PC_DBUS_LIBRARIES} )

## Add headers for allowing IDEs to know them - not needed for building actually.
set(LIBSOURCES ${CMAKE_SOURCE_DIR}/include/libskype.h )

include_directories( ${CMAKE_SOURCE_DIR}/include )

add_subdirectory( src )
