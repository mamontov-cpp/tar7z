cmake_minimum_required(VERSION 2.8.12)
project(tar7z-tests)

file(GLOB_RECURSE HDRS ../include/*.h)
find_package(Boost REQUIRED)

set(TAR7Z_EXECUTABLE_NAME "tar7z-tests")
set(TAR7Z_LINKABLE_NAME "tar7z")

if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Release")
	set(CMAKE_BUILD_TYPE "Release")
	set(TAR7Z_LINKABLE_NAME "${TAR7Z_LIBRARY_NAME}-release")
else()	
	string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWERCASED)
	set(TAR7Z_LINKABLE_NAME "tar7z-${CMAKE_BUILD_TYPE_LOWERCASED}")
endif()

include_directories(../include)
include_directories(${Boost_INCLUDE_DIRS})

link_directories("../lib")

set(SRCS "main.cpp" "writer.cpp")

add_executable(${TAR7Z_EXECUTABLE_NAME} ${SRCS} ${HDRS})

target_link_libraries(${TAR7Z_EXECUTABLE_NAME} ${TAR7Z_LINKABLE_NAME})

set_target_properties(${TAR7Z_EXECUTABLE_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "../lib"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "../lib"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "../lib"	
    LIBRARY_OUTPUT_DIRECTORY "../lib"
	LIBRARY_OUTPUT_DIRECTORY_DEBUG "../lib"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "../lib"	
    RUNTIME_OUTPUT_DIRECTORY "../bin"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "../bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "../bin"
	DEBUG_POSTFIX "-debug"
	RELEASE_POSTFIX "-release"
)
