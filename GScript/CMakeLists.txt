cmake_minimum_required(VERSION 3.0)

message(STATUS "---------------------------------------")
message(STATUS "Main CMakeLists file for GScript")
message(STATUS "---------------------------------------")

set(GSCRIPT_ROOT ${GFRAMEWORK_ROOT}/GScript)

project(GScript)

list(APPEND include_paths ${GSCRIPT_ROOT}/../GScript)
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
	${GSCRIPT_ROOT}/GFrameworkLua.h
	${GSCRIPT_ROOT}/GLua_data_exchange.h
	${GSCRIPT_ROOT}/GLuaScript.h
	${GSCRIPT_ROOT}/GScript.h
	
)

file(
	GLOB
	source_files
	${GSCRIPT_ROOT}/GLua_data_exchange.cpp
	${GSCRIPT_ROOT}/GLuaScript.cpp
	${GSCRIPT_ROOT}/GScript.cpp
	${THIRDPARTY_LUA_PATH}/lapi.c ${THIRDPARTY_LUA_PATH}/lcode.c ${THIRDPARTY_LUA_PATH}/lctype.c ${THIRDPARTY_LUA_PATH}/ldebug.c ${THIRDPARTY_LUA_PATH}/ldo.c 
	${THIRDPARTY_LUA_PATH}/ldump.c ${THIRDPARTY_LUA_PATH}/lfunc.c ${THIRDPARTY_LUA_PATH}/lgc.c ${THIRDPARTY_LUA_PATH}/llex.c ${THIRDPARTY_LUA_PATH}/lmem.c 
	${THIRDPARTY_LUA_PATH}/lobject.c ${THIRDPARTY_LUA_PATH}/lopcodes.c ${THIRDPARTY_LUA_PATH}/lparser.c ${THIRDPARTY_LUA_PATH}/lstate.c ${THIRDPARTY_LUA_PATH}/lstring.c 
	${THIRDPARTY_LUA_PATH}/ltable.c ${THIRDPARTY_LUA_PATH}/ltm.c ${THIRDPARTY_LUA_PATH}/lundump.c ${THIRDPARTY_LUA_PATH}/lvm.c ${THIRDPARTY_LUA_PATH}/lzio.c
	${THIRDPARTY_LUA_PATH}/lauxlib.c ${THIRDPARTY_LUA_PATH}/lbaselib.c ${THIRDPARTY_LUA_PATH}/lbitlib.c ${THIRDPARTY_LUA_PATH}/lcorolib.c ${THIRDPARTY_LUA_PATH}/ldblib.c 
	${THIRDPARTY_LUA_PATH}/liolib.c ${THIRDPARTY_LUA_PATH}/lmathlib.c ${THIRDPARTY_LUA_PATH}/loslib.c ${THIRDPARTY_LUA_PATH}/lstrlib.c ${THIRDPARTY_LUA_PATH}/ltablib.c 
	${THIRDPARTY_LUA_PATH}/lutf8lib.c ${THIRDPARTY_LUA_PATH}/loadlib.c 	${THIRDPARTY_LUA_PATH}/linit.c
)

# file(
	# GLOB_RECURSE
	# source_files
	# ${THIRDPARTY_LUA_PATH}/*.c
	# )
	
add_library(
	gscript_shared
	OBJECT
	${header_files}
	${source_files}
)
set_property(TARGET gscript_shared PROPERTY CXX_STANDARD 14)


include_directories(${GFRAMEWORK_ROOT}/../)
include_directories(${include_paths})

add_library(
  gscript_static
  OBJECT
  ${header_files}
  ${source_files}
)
set_property(TARGET gscript_static PROPERTY CXX_STANDARD 14)
if(${GFRAMEWORK_TARGET_OS} MATCHES "win")
	target_compile_definitions(gscript_shared PUBLIC -DLUA_BUILD_AS_DLL)
endif(${GFRAMEWORK_TARGET_OS} MATCHES "win")
target_compile_definitions(gscript_shared PUBLIC -DLUA_LIB)
set_output_paths(gscript)


install	(	DIRECTORY ${GSCRIPT_ROOT}
			DESTINATION ./include/GFramework
			FILES_MATCHING PATTERN "*.h"
		)
		
set (gscript.include_files_list ${header_files} PARENT_SCOPE)
set (gscript.include_paths_list ${include_paths} PARENT_SCOPE)
set (gscript.static.compile_defines "" PARENT_SCOPE)
set (gscript.static.compile_libs "" PARENT_SCOPE)
set (gscript.static.library_paths_list "" PARENT_SCOPE)
set (gscript.static.lib_name gscript_static PARENT_SCOPE)

