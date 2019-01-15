cmake_minimum_required(VERSION 3.0)

message(STATUS "---------------------------------------")
message(STATUS "Main CMakeLists file for GScript")
message(STATUS "---------------------------------------")

set(GSCRIPT_ROOT ${GFRAMEWORK_ROOT}/GScript)

project(GScript)

list(APPEND include_paths ${GSCRIPT_ROOT}/../GScript)
list(APPEND include_paths ${THIRDPARTY_LUA_PATH}/src)
list(APPEND include_paths ${THIRDPARTY_BOOST_PATH})
list(APPEND include_paths ${THIRDPARTY_GLM_PATH})

file(
	GLOB
	header_files
	${GSCRIPT_ROOT}/GFrameworkLua.h
	${GSCRIPT_ROOT}/GLua_data_exchange.h
	${GSCRIPT_ROOT}/GLuaScript.h
)

file(
	GLOB
	source_files
	${GSCRIPT_ROOT}/GLua_data_exchange.cpp
	${GSCRIPT_ROOT}/GLuaScript.cpp
	${THIRDPARTY_LUA_PATH}/src/lapi.c ${THIRDPARTY_LUA_PATH}/src/lcode.c ${THIRDPARTY_LUA_PATH}/src/lctype.c ${THIRDPARTY_LUA_PATH}/src/ldebug.c ${THIRDPARTY_LUA_PATH}/src/ldo.c 
	${THIRDPARTY_LUA_PATH}/src/ldump.c ${THIRDPARTY_LUA_PATH}/src/lfunc.c ${THIRDPARTY_LUA_PATH}/src/lgc.c ${THIRDPARTY_LUA_PATH}/src/llex.c ${THIRDPARTY_LUA_PATH}/src/lmem.c 
	${THIRDPARTY_LUA_PATH}/src/lobject.c ${THIRDPARTY_LUA_PATH}/src/lopcodes.c ${THIRDPARTY_LUA_PATH}/src/lparser.c ${THIRDPARTY_LUA_PATH}/src/lstate.c ${THIRDPARTY_LUA_PATH}/src/lstring.c 
	${THIRDPARTY_LUA_PATH}/src/ltable.c ${THIRDPARTY_LUA_PATH}/src/ltm.c ${THIRDPARTY_LUA_PATH}/src/lundump.c ${THIRDPARTY_LUA_PATH}/src/lvm.c ${THIRDPARTY_LUA_PATH}/src/lzio.c
	${THIRDPARTY_LUA_PATH}/src/lauxlib.c ${THIRDPARTY_LUA_PATH}/src/lbaselib.c ${THIRDPARTY_LUA_PATH}/src/lbitlib.c ${THIRDPARTY_LUA_PATH}/src/lcorolib.c ${THIRDPARTY_LUA_PATH}/src/ldblib.c 
	${THIRDPARTY_LUA_PATH}/src/liolib.c ${THIRDPARTY_LUA_PATH}/src/lmathlib.c ${THIRDPARTY_LUA_PATH}/src/loslib.c ${THIRDPARTY_LUA_PATH}/src/lstrlib.c ${THIRDPARTY_LUA_PATH}/src/ltablib.c 
	${THIRDPARTY_LUA_PATH}/src/lutf8lib.c ${THIRDPARTY_LUA_PATH}/src/loadlib.c 	${THIRDPARTY_LUA_PATH}/src/linit.c
)

# file(
	# GLOB_RECURSE
	# source_files
	# ${THIRDPARTY_LUA_PATH}/src/*.c
	# )
	
add_library(
	gscript_shared
	OBJECT
	${header_files}
	${source_files}
)
set_property(TARGET gscript_shared PROPERTY CXX_STANDARD 14)


include_directories(${GFRAMEWORK_ROOT})
include_directories(${include_paths})

add_library(
  gscript_static
  OBJECT
  ${header_files}
  ${source_files}
)
set_property(TARGET gscript_static PROPERTY CXX_STANDARD 14)

set_output_paths(gscript)


install	(	DIRECTORY ${GSCRIPT_ROOT}
			DESTINATION ./include
			FILES_MATCHING PATTERN "*.h"
		)
		
set (gscript.include_files_list ${header_files} PARENT_SCOPE)
set (gscript.include_paths_list ${include_paths} PARENT_SCOPE)
set (gscript.static.compile_defines "" PARENT_SCOPE)
set (gscript.static.compile_libs "" PARENT_SCOPE)
set (gscript.static.library_paths_list "" PARENT_SCOPE)
set (gscript.static.lib_name gscript_static PARENT_SCOPE)

