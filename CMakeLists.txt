cmake_minimum_required(VERSION 3.1)

message(STATUS "-----------------------------------")
message(STATUS "Main CMakeLists file for GFramework")
message(STATUS "-----------------------------------")

set(GFRAMEWORK_ROOT ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "GFramework Root build path")

# CMake options
set(CMAKE_MODULE_PATH ${GFRAMEWORK_ROOT}/cmake ${CMAKE_MODULE_PATH})

if(${GFRAMEWORK_TARGET_OS_ARCH} MATCHES "win_32")
	#No toolchain file for win32 because it is a native build
	set(TARGET_SHARED_LIBRARY_EXT dll)
	set(TARGET_SCRIPT_EXT bat)
elseif(${GFRAMEWORK_TARGET_OS_ARCH} MATCHES "win_64")
	#No toolchain file for win32 because it is a native build
	set(TARGET_SHARED_LIBRARY_EXT dll)
	set(TARGET_SCRIPT_EXT bat)	
elseif(${GFRAMEWORK_TARGET_OS_ARCH} MATCHES "linux_x86_64")
	#No toolchain file for Linux x86_64 because it is a native build
	set(TARGET_SHARED_LIBRARY_EXT so)
	set(TARGET_SCRIPT_EXT sh)
endif(${GFRAMEWORK_TARGET_OS_ARCH} MATCHES "win_32")

project(GFramework)

include(toolchain_utilities)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/output/GFramework CACHE PATH "Select the install path for GFramework" FORCE)

# Host PC configuration
set(HOST_SCRIPT_EXT bat)
set(HOSTNAME_PREFIX )
string(SUBSTRING ${CMAKE_HOST_SYSTEM} 0 5 HOSTNAME_PREFIX)
if(${HOSTNAME_PREFIX} MATCHES "Linux")
	if(NOT ${GFRAMEWORK_TARGET} MATCHES "linux_x86_64")
		message(FATAL_ERROR "From linux PC, cross compilation is not allowed! Set the GFRAMEWORK_TARGET to linux_x86_64")
	endif(NOT ${GFRAMEWORK_TARGET} MATCHES "linux_x86_64")
	set(HOST_SCRIPT_EXT sh)
endif(${HOSTNAME_PREFIX} MATCHES "Linux")

# get access to cmake utilities
#include(utilities)

# get access to our actual CMake file
#include(cpackconfig)

# third party libraries
set(THIRDPARTY_LUA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lua-5.3.4 CACHE PATH "Lua path")
set(THIRDPARTY_GLM_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm_0_9_8_3 CACHE PATH "glm path")
set(THIRDPARTY_BOOST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_63_0 CACHE PATH "Boost path")

add_subdirectory(GVariant)
add_subdirectory(GSerialization)
add_subdirectory(GReflection)
add_subdirectory(GScript)

add_library(
	gframework_static
	STATIC
	$<TARGET_OBJECTS:gvariant_static>
	$<TARGET_OBJECTS:gserialization_static>
	$<TARGET_OBJECTS:greflection_static>
	$<TARGET_OBJECTS:gscript_static>
)

add_library(
	gframework_shared
	SHARED
	$<TARGET_OBJECTS:gvariant_shared>
	$<TARGET_OBJECTS:gserialization_shared>
	$<TARGET_OBJECTS:greflection_shared>
	$<TARGET_OBJECTS:gscript_shared>
)

set(GFRAMEWORK_BUILD_TEST OFF CACHE BOOL "Select to build the test for Gframework")
		
if(${GFRAMEWORK_BUILD_TEST} MATCHES ON)
	add_subdirectory(UnitTest)
endif(${GFRAMEWORK_BUILD_TEST} MATCHES ON)
		
install	(	TARGETS gframework_static gframework_shared
			CONFIGURATIONS Debug
			RUNTIME DESTINATION ./bin/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug
			LIBRARY DESTINATION ./lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug
			ARCHIVE DESTINATION ./lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug
		)

install	(	TARGETS gframework_static gframework_shared
			CONFIGURATIONS Release
			RUNTIME DESTINATION ./bin/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release
			LIBRARY DESTINATION ./lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release
			ARCHIVE DESTINATION ./lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release
		)
#add_subdirectory(Test)

