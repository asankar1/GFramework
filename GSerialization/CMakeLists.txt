cmake_minimum_required(VERSION 3.0)

message(STATUS "---------------------------------------")
message(STATUS "Main CMakeLists file for GSerialization")
message(STATUS "---------------------------------------")

set(GSERIALIZATION_ROOT ${GFRAMEWORK_ROOT}/GSerialization)

project(GSerialization)

list(APPEND include_paths ${GSERIALIZATION_ROOT}/../GSerialization)
list(APPEND include_paths ${THIRDPARTY_LUA_PATH})
list(APPEND include_paths ${THIRDPARTY_GLM_PATH})

find_package(Boost 1.63.0) 
if(Boost_FOUND)
	list(APPEND include_paths ${Boost_INCLUDE_DIRS})
else()
	list(APPEND include_paths ${THIRDPARTY_BOOST_PATH})
endif()
file(
	GLOB
	header_files
	${GSERIALIZATION_ROOT}/GSerializer.h
	${GSERIALIZATION_ROOT}/GStringSerializer.h
)

file(
	GLOB
	source_files
	${GSERIALIZATION_ROOT}/GSerializer.cpp
	${GSERIALIZATION_ROOT}/GStringSerializer.cpp
)

add_library(
	gserialization_shared
	OBJECT
	${header_files}
	${source_files}
)
set_property(TARGET gserialization_shared PROPERTY CXX_STANDARD 14)


include_directories(${GFRAMEWORK_ROOT}/../)
include_directories(${include_paths})

add_library(
  gserialization_static
  OBJECT
  ${header_files}
  ${source_files}
)
set_property(TARGET gserialization_static PROPERTY CXX_STANDARD 14)

set_output_paths(gserialization)


install	(	DIRECTORY ${GSERIALIZATION_ROOT}
			DESTINATION ./include/GFramework
			FILES_MATCHING PATTERN "*.h"
		)
		
set (gserialization.include_files_list ${header_files} PARENT_SCOPE)
set (gserialization.include_paths_list ${include_paths} PARENT_SCOPE)
set (gserialization.static.compile_defines "" PARENT_SCOPE)
set (gserialization.static.compile_libs "" PARENT_SCOPE)
set (gserialization.static.library_paths_list "" PARENT_SCOPE)
set (gserialization.static.lib_name gserialization_static PARENT_SCOPE)

