cmake_minimum_required(VERSION 3.0)

message(STATUS "---------------------------------------")
message(STATUS "Main CMakeLists file for GReflection")
message(STATUS "---------------------------------------")

set(GREFLECTION_ROOT ${GFRAMEWORK_ROOT}/GReflection)

project(GReflection)

list(APPEND include_paths ${GREFLECTION_ROOT}/../GReflection)
list(APPEND include_paths ${THIRDPARTY_LUA_PATH}/src)
list(APPEND include_paths ${THIRDPARTY_BOOST_PATH})
list(APPEND include_paths ${THIRDPARTY_GLM_PATH})

file(
	GLOB
	header_files
	${GREFLECTION_ROOT}/GReflection.h
)

file(
	GLOB
	source_files
	${GREFLECTION_ROOT}/GReflection.cpp
)

add_library(
	greflection_shared
	OBJECT
	${header_files}
	${source_files}
)
set_property(TARGET greflection_shared PROPERTY CXX_STANDARD 14)


include_directories(${GFRAMEWORK_ROOT})
include_directories(${include_paths})

add_library(
  greflection_static
  OBJECT
  ${header_files}
  ${source_files}
)
set_property(TARGET greflection_static PROPERTY CXX_STANDARD 14)

set_output_paths(greflection)


install	(	DIRECTORY ${GREFLECTION_ROOT}
			DESTINATION ./include
			FILES_MATCHING PATTERN "*.h"
		)

		
set (greflection.include_files_list ${header_files} PARENT_SCOPE)
set (greflection.include_paths_list ${include_paths} PARENT_SCOPE)
set (greflection.static.compile_defines "" PARENT_SCOPE)
set (greflection.static.compile_libs "" PARENT_SCOPE)
set (greflection.static.library_paths_list "" PARENT_SCOPE)
set (greflection.static.lib_name greflection_static PARENT_SCOPE)

