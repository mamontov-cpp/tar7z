cmake_minimum_required(VERSION 2.8.12)
project(tar7z)

find_package(Boost REQUIRED)

file(GLOB_RECURSE SRCS src/*.cpp src/*.cc)
file(GLOB_RECURSE HDRS include/*.h)

set(TAR7Z_LIBRARY_NAME "tar7z")

set(TAR7Z_CXX_DEBUG_FLAGS "-Wno-reorder -Wno-unused -Wno-sign-compare -w")
set(TAR7Z_CXX_RELEASE_FLAGS "-O2 -Wno-reorder -Wno-unused -Wno-sign-compare -w")

if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Release")
	set(CMAKE_BUILD_TYPE "Release")
	set(TAR7Z_LIBRARY_NAME "${TAR7Z_LIBRARY_NAME}")
endif()

macro(SET_GCC_FLAGS)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${TAR7Z_CXX_DEBUG_FLAGS}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${TAR7Z_CXX_RELEASE_FLAGS}")
	if (NOT CMAKE_BUILD_TYPE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TAR7Z_CXX_RELEASE_FLAGS}")
	endif()
endmacro(SET_GCC_FLAGS)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})

IF (WIN32)
  add_definitions(-DWIN32)
  IF (MINGW)
	add_definitions(-DMINGW)	
	SET_GCC_FLAGS()
  ENDIF()
  IF (MSVC)
	add_definitions(-DCRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_DEPRECATE)
  ENDIF()
ELSE()
  add_definitions(-DUNIX -DLINUX -DGCC)
  SET_GCC_FLAGS()
ENDIF()



add_library(${TAR7Z_LIBRARY_NAME}  ${SRCS} ${HDRS})

set_target_properties(${TAR7Z_LIBRARY_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "lib"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "lib"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "lib"	
    LIBRARY_OUTPUT_DIRECTORY "lib"
	LIBRARY_OUTPUT_DIRECTORY_DEBUG "lib"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "lib"	
    RUNTIME_OUTPUT_DIRECTORY "bin"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "bin"
	DEBUG_POSTFIX "-debug"
	RELEASE_POSTFIX "-release"
)

add_subdirectory("tests")
