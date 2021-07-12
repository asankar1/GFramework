cmake_minimum_required(VERSION 3.0)

message(STATUS "---------------------------------")
message(STATUS "Main CMakeLists file for GVariant")
message(STATUS "---------------------------------")

set(GVARIANT_ROOT ${GFRAMEWORK_ROOT}/GVariant)

project(GVariant)

list(APPEND include_paths ${GVARIANT_ROOT}/../GSerialization)
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
	${GVARIANT_ROOT}/GTypes.h
	${GVARIANT_ROOT}/GProperty.h
	${GVARIANT_ROOT}/GVariant.h
	${GVARIANT_ROOT}/GObject.h
)

file(
	GLOB
	source_files
	${GVARIANT_ROOT}/GVariant.cpp
	${GVARIANT_ROOT}/GProperty.cpp
	${GVARIANT_ROOT}/GObject.cpp
)

add_library(
  gvariant_shared
  OBJECT
  ${header_files}
  ${source_files}
)

set_property(TARGET gvariant_shared PROPERTY CXX_STANDARD 14)

include_directories(${GFRAMEWORK_ROOT}/../)
include_directories(${include_paths})

add_library(
  gvariant_static
  OBJECT
  ${header_files}
  ${source_files}
)
set_property(TARGET gvariant_static PROPERTY CXX_STANDARD 14)

set_output_paths(gvariant)


install	(	DIRECTORY ${GVARIANT_ROOT}
			DESTINATION ./include/GFramework
			FILES_MATCHING PATTERN "*.h"
		)
		
set (gvariant.include_files_list ${header_files} PARENT_SCOPE)
set (gvariant.include_paths_list ${include_paths} PARENT_SCOPE)
set (gvariant.static.compile_defines "" PARENT_SCOPE)
set (gvariant.static.compile_libs "" PARENT_SCOPE)
set (gvariant.static.library_paths_list "" PARENT_SCOPE)
set (gvariant.static.lib_name gvariant_static PARENT_SCOPE)

