cmake_minimum_required(VERSION 3.0)

message(STATUS "----------------------------------------")
message(STATUS "Main CMakeLists file for GFramework Test")
message(STATUS "----------------------------------------")

set(GFRAMEWORK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../Build/output CACHE PATH "GFramework path")
set(TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${TEST_ROOT}/../cmake ${CMAKE_MODULE_PATH})
set(THIRDPARTY_LUA_PATH ${TEST_ROOT}/../third_party/lua-5.3.4 CACHE PATH "Lua path")
set(THIRDPARTY_GLM_PATH ${TEST_ROOT}/../third_party/glm_0_9_8_3 CACHE PATH "glm path")
set(THIRDPARTY_BOOST_PATH ${TEST_ROOT}/../third_party/boost_1_63_0 CACHE PATH "Boost path")

project(GFramework_Test)

include(toolchain_utilities)
#include(utilities)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/../Build/output CACHE PATH "Select the install path for GFramework" FORCE)

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
	${TEST_ROOT}/gvariant_test.cpp
	${TEST_ROOT}/gproperty_test.cpp
	${TEST_ROOT}/main.cpp
)

include_directories(${GFRAMEWORK_PATH}/include)
include_directories(${TEST_ROOT})
include_directories(${THIRDPARTY_LUA_PATH}/src)
include_directories(${THIRDPARTY_BOOST_PATH})
include_directories(${THIRDPARTY_GLM_PATH})


if("${CMAKE_BUILD_TYPE} " STREQUAL " ")
	set(GFRAMEWORK_BUILD_TYPE "$(Configuration)")
else()
	set(GFRAMEWORK_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
endif("${CMAKE_BUILD_TYPE} " STREQUAL " ")
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message(STATUS "GFRAMEWORK_BUILD_TYPE:${GFRAMEWORK_BUILD_TYPE}")
link_directories("${GFRAMEWORK_PATH}/lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_${GFRAMEWORK_BUILD_TYPE}")

add_executable(
	gframework_static_test
	${header_files}
	${source_files}
)

add_executable(
	gframework_shared_test
	${header_files}
	${source_files}
)

target_link_libraries (
	gframework_static_test
	gframework_static
)

target_link_libraries (
	gframework_shared_test
	gframework_shared
)

target_compile_definitions(gframework_static_test PUBLIC GFRAMEWORK_API=)
target_compile_definitions(gframework_shared_test PUBLIC GFRAMEWORK_API=${GFRAMEWORK_IMPORT_API})

set_property(TARGET gframework_static_test PROPERTY CXX_STANDARD 14)
set_property(TARGET gframework_shared_test PROPERTY CXX_STANDARD 14)

install	(	TARGETS gframework_static_test gframework_shared_test 
			CONFIGURATIONS Debug
			RUNTIME DESTINATION ./test/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug
		)
		
install	(	DIRECTORY "${GFRAMEWORK_PATH}/lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug/"
			CONFIGURATIONS Debug
			DESTINATION ./test/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug
			FILES_MATCHING PATTERN "*.${GFRAMEWORK_SHARED_LIB_EXT}"
		)
		
install	(	TARGETS gframework_static_test gframework_shared_test 
			CONFIGURATIONS Release
			RUNTIME DESTINATION ./test/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release
		)
		
install	(	DIRECTORY "${GFRAMEWORK_PATH}/lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release/"
			CONFIGURATIONS Release
			DESTINATION ./test/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release
			FILES_MATCHING PATTERN "*.${GFRAMEWORK_SHARED_LIB_EXT}"
		)		