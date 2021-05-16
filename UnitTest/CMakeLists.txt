cmake_minimum_required(VERSION 3.8)

message(STATUS "----------------------------------------")
message(STATUS "Main CMakeLists file for GFramework Test")
message(STATUS "----------------------------------------")

set(TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(GFRAMEWORK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../output/GFramework)
set(CMAKE_MODULE_PATH ${TEST_ROOT}/../cmake ${CMAKE_MODULE_PATH})
set(THIRDPARTY_LUA_PATH ${TEST_ROOT}/../third_party/lua-5.3.4 CACHE PATH "Lua path")
set(THIRDPARTY_GLM_PATH ${TEST_ROOT}/../third_party/glm_0_9_8_3 CACHE PATH "glm path")
set(THIRDPARTY_BOOST_PATH ${TEST_ROOT}/../third_party/boost_1_63_0 CACHE PATH "Boost path")

project(GFramework_Test)

include(toolchain_utilities)
#include(utilities)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/output/GFramework CACHE PATH "Select the install path for GFramework" FORCE)

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
	${TEST_ROOT}/greflection_test.cpp
	${TEST_ROOT}/gserialization_test.cpp
	${TEST_ROOT}/gscript_test.cpp
	#${TEST_ROOT}/Node.cpp
	#${TEST_ROOT}/Sphere.cpp
)


include_directories(${TEST_ROOT})
include_directories(${GFRAMEWORK_PATH}/include)
include_directories(${THIRDPARTY_LUA_PATH}/src)
include_directories(${THIRDPARTY_BOOST_PATH})
include_directories(${THIRDPARTY_GLM_PATH})
link_directories(${GFRAMEWORK_PATH}/lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_$(Configuration))

if("${CMAKE_BUILD_TYPE} " STREQUAL " ")
	set(GFRAMEWORK_BUILD_TYPE "$(Configuration)")
else()
	set(GFRAMEWORK_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
endif("${CMAKE_BUILD_TYPE} " STREQUAL " ")
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message(STATUS "GFRAMEWORK_BUILD_TYPE:${GFRAMEWORK_BUILD_TYPE}")

add_library(
	gframework_test_shared_lib
	SHARED
	${header_files}
	${source_files}
)

add_library(
	gframework_test_static_lib
	STATIC
	${header_files}
	${source_files}
)

add_executable(
	gframework_shared_test
	${TEST_ROOT}/main.cpp
)

add_executable(
	gframework_static_test
	${TEST_ROOT}/main.cpp
)

target_link_libraries (
	gframework_static_test
	gframework_test_static_lib
	gframework_static
)

target_link_libraries (
	gframework_shared_test
	gframework_test_shared_lib
)

target_link_libraries (
	gframework_test_shared_lib
	gframework_shared
)

target_compile_definitions(gframework_static_test PUBLIC GFRAMEWORK_API=)
target_compile_definitions(gframework_shared_test PUBLIC GFRAMEWORK_API=${GFRAMEWORK_IMPORT_API})
target_compile_definitions(gframework_test_static_lib PUBLIC GFRAMEWORK_API=)	
target_compile_definitions(gframework_test_shared_lib PUBLIC GFRAMEWORK_API=${GFRAMEWORK_IMPORT_API})

target_compile_definitions(gframework_static_test PUBLIC GFRAMEWORK_TEST_API=)
target_compile_definitions(gframework_shared_test PUBLIC GFRAMEWORK_TEST_API=${GFRAMEWORK_IMPORT_API})
target_compile_definitions(gframework_test_static_lib PUBLIC GFRAMEWORK_TEST_API=)
target_compile_definitions(gframework_test_shared_lib PUBLIC GFRAMEWORK_TEST_API=${GFRAMEWORK_EXPORT_API})


set_property(TARGET gframework_static_test PROPERTY CXX_STANDARD 14)
set_property(TARGET gframework_shared_test PROPERTY CXX_STANDARD 14)

set_property(TARGET gframework_static_test PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/test/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_$(Configuration))
set_property(TARGET gframework_shared_test PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/test/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_$(Configuration))

install	(	TARGETS gframework_static_test gframework_shared_test gframework_test_shared_lib
			CONFIGURATIONS Debug
			RUNTIME DESTINATION ./test/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug
		)
		
install	(	DIRECTORY "${GFRAMEWORK_PATH}/lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug/"
						"${GFRAMEWORK_PATH}/bin/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug/"
			CONFIGURATIONS Debug
			DESTINATION ./test/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug
			FILES_MATCHING PATTERN "*.${GFRAMEWORK_SHARED_LIB_EXT}"
		)
		
install	(	TARGETS gframework_static_test gframework_shared_test gframework_test_shared_lib
			CONFIGURATIONS Release
			RUNTIME DESTINATION ./test/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release
		)
		
install	(	DIRECTORY "${GFRAMEWORK_PATH}/lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release/"
			CONFIGURATIONS Release
			DESTINATION ./test/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release
			FILES_MATCHING PATTERN "*.${GFRAMEWORK_SHARED_LIB_EXT}"
		)		