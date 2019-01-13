cmake_minimum_required(VERSION 3.0)

message(STATUS "---------------------------------")
message(STATUS "Main CMakeLists file for GVariant")
message(STATUS "---------------------------------")

set(GVARIANT_ROOT ${GFRAMEWORK_ROOT}/GVariant)

project(GVariant)

list(APPEND include_paths ${GVARIANT_ROOT}/../GSerialization)
list(APPEND include_paths ${THIRDPARTY_LUA_PATH}/src)
list(APPEND include_paths ${THIRDPARTY_BOOST_PATH})
list(APPEND include_paths ${THIRDPARTY_GLM_PATH})

file(
	GLOB
	header_files
	${GVARIANT_ROOT}/GTypes.h
	${GVARIANT_ROOT}/GProperty.h
	${GVARIANT_ROOT}/GVariant.h
)

file(
	GLOB
	source_files
	${GVARIANT_ROOT}/GVariant.cpp
	${GVARIANT_ROOT}/GProperty.cpp
)

add_library(
  gvariant_shared
  SHARED
  ${header_files}
  ${source_files}
)
set_property(TARGET gvariant_shared PROPERTY CXX_STANDARD 11)


include_directories(${GFRAMEWORK_ROOT})
include_directories(${include_paths})

add_library(
  gvariant_static
  STATIC
  ${header_files}
  ${source_files}
)
set_property(TARGET gvariant_static PROPERTY CXX_STANDARD 11)

set_output_paths(gvariant)


install	(	DIRECTORY ${GVARIANT_ROOT}
			DESTINATION ./include
			FILES_MATCHING PATTERN "*.h"
		)
		
install	(	TARGETS gvariant_static gvariant_shared 
			CONFIGURATIONS Debug
			RUNTIME DESTINATION ./lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug
			LIBRARY DESTINATION ./lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug
			ARCHIVE DESTINATION ./lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug
		)

install	(	TARGETS gvariant_static gvariant_shared 
			CONFIGURATIONS Release
			RUNTIME DESTINATION ./lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release
			LIBRARY DESTINATION ./lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release
			ARCHIVE DESTINATION ./lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release
		)
		
set (gvariant.include_files_list ${header_files} PARENT_SCOPE)
set (gvariant.include_paths_list ${include_paths} PARENT_SCOPE)
set (gvariant.static.compile_defines "" PARENT_SCOPE)
set (gvariant.static.compile_libs "" PARENT_SCOPE)
set (gvariant.static.library_paths_list "" PARENT_SCOPE)
set (gvariant.static.lib_name gvariant_static PARENT_SCOPE)

