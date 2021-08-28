cmake_minimum_required(VERSION 3.8)

message(STATUS "----------------------------------------")
message(STATUS "Main CMakeLists file for GFramework Test")
message(STATUS "----------------------------------------")

set(TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(GFRAMEWORK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../output/GFramework)
set(CMAKE_MODULE_PATH ${TEST_ROOT}/../cmake ${CMAKE_MODULE_PATH})
set(THIRDPARTY_LUA_PATH ${TEST_ROOT}/../third_party/lua CACHE PATH "Lua 5.3.4 path")
set(THIRDPARTY_GLM_PATH ${TEST_ROOT}/../third_party/glm CACHE PATH "glm 9.8 path")
set(THIRDPARTY_BOOST_PATH ${TEST_ROOT}/../third_party/boost CACHE PATH "Boost 1.63 path")

if (NOT DEFINED CMAKE_INSTALL_PREFIX)
	set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/output/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER} CACHE PATH "Select the install path for GFramework" FORCE)
endif()

if(${GENERATOR_IS_MULTI_CONFIG} MATCHES "True")
	message(STATUS "doing GENERATOR_IS_MULTI_CONFIG")
	set(BUILD_CONFIGURATION $(Configuration))
else()	
	message(STATUS "Not doing GENERATOR_IS_MULTI_CONFIG")
	set(BUILD_CONFIGURATION ${CMAKE_BUILD_TYPE})
endif()

if("${CMAKE_BUILD_TYPE} " STREQUAL " ")
	set(GFRAMEWORK_BUILD_TYPE "$(Configuration)")
else()
	set(GFRAMEWORK_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
endif("${CMAKE_BUILD_TYPE} " STREQUAL " ")
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message(STATUS "GFRAMEWORK_BUILD_TYPE:${GFRAMEWORK_BUILD_TYPE}")


include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

project(GFramework_Test)

include(toolchain_utilities)
#include(utilities)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

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
	#${TEST_ROOT}/gvariant_test.cpp
	#${TEST_ROOT}/gproperty_test.cpp
	#${TEST_ROOT}/greflection_test.cpp
	#${TEST_ROOT}/gserialization_test.cpp
	${TEST_ROOT}/gscript_test.cpp
	#${TEST_ROOT}/Node.cpp
	#${TEST_ROOT}/Sphere.cpp
)


include_directories(${TEST_ROOT})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../)
include_directories(${THIRDPARTY_LUA_PATH})
include_directories(${THIRDPARTY_BOOST_PATH})
include_directories(${THIRDPARTY_GLM_PATH})
include_directories(${googletest_SOURCE_DIR}/googletest)
include_directories(${googletest_SOURCE_DIR}/googletest/include)
link_directories("${GFRAMEWORK_PATH}/lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_${GFRAMEWORK_BUILD_TYPE}")




# add_library(
	# gframework_test_shared_lib
	# SHARED
	# ${header_files}
	# ${source_files}
# )

# add_library(
	# gframework_test_static_lib
	# STATIC
	# ${header_files}
	# ${source_files}
# )

add_executable(
	gframework_shared_test
	${TEST_ROOT}/main.cpp
	${header_files}
	${source_files}

)

add_executable(
	gframework_static_test
	${TEST_ROOT}/main.cpp
	${header_files}
	${source_files}

)

target_link_libraries (
	gframework_static_test
	gframework_static
	gtest_main
)

target_link_libraries (
	gframework_shared_test
	gframework_shared
	gtest_main
)

# target_link_libraries (
	# gframework_test_shared_lib
	# gframework_shared
	# gtest_main
# )

target_compile_definitions(gframework_static_test PUBLIC GFRAMEWORK_API=)
target_compile_definitions(gframework_shared_test PUBLIC GFRAMEWORK_API=${GFRAMEWORK_IMPORT_API})
# target_compile_definitions(gframework_test_static_lib PUBLIC GFRAMEWORK_API=)	
# target_compile_definitions(gframework_test_shared_lib PUBLIC GFRAMEWORK_API=${GFRAMEWORK_IMPORT_API})

target_compile_definitions(gframework_static_test PUBLIC GFRAMEWORK_TEST_API=)
target_compile_definitions(gframework_shared_test PUBLIC GFRAMEWORK_TEST_API=)
# target_compile_definitions(gframework_test_static_lib PUBLIC GFRAMEWORK_TEST_API=)
# target_compile_definitions(gframework_test_shared_lib PUBLIC GFRAMEWORK_TEST_API=${GFRAMEWORK_EXPORT_API})


set_property(TARGET gframework_static_test PROPERTY CXX_STANDARD 14)
set_property(TARGET gframework_shared_test PROPERTY CXX_STANDARD 14)

set_property(TARGET gframework_static_test PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/test/$(Configuration))
set_property(TARGET gframework_shared_test PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/test/$(Configuration))

install	(	TARGETS gframework_static_test gframework_shared_test
			CONFIGURATIONS Debug
			RUNTIME DESTINATION ./test/Debug
		)
		
install	(	DIRECTORY "${GFRAMEWORK_PATH}/lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug/"
						"${GFRAMEWORK_PATH}/bin/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Debug/"
			CONFIGURATIONS Debug
			DESTINATION ./test/Debug
			FILES_MATCHING PATTERN "*.${GFRAMEWORK_SHARED_LIB_EXT}"
		)
		
install	(	TARGETS gframework_static_test gframework_shared_test
			CONFIGURATIONS RelWithDebInfo
			RUNTIME DESTINATION ./test/RelWithDebInfo
		)
		
install	(	DIRECTORY "${GFRAMEWORK_PATH}/lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_RelWithDebInfo/"
						"${GFRAMEWORK_PATH}/bin/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_RelWithDebInfo/"
			CONFIGURATIONS RelWithDebInfo
			DESTINATION ./test/RelWithDebInfo
			FILES_MATCHING PATTERN "*.${GFRAMEWORK_SHARED_LIB_EXT}"
		)			
		
install	(	TARGETS gframework_static_test gframework_shared_test
			CONFIGURATIONS Release
			RUNTIME DESTINATION ./test/Release
		)
		
install	(	DIRECTORY "${GFRAMEWORK_PATH}/lib/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release/"
						"${GFRAMEWORK_PATH}/bin/${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER}_Release/"
			CONFIGURATIONS Release
			DESTINATION ./test/Release
			FILES_MATCHING PATTERN "*.${GFRAMEWORK_SHARED_LIB_EXT}"
		)		
		
install	(	FILES "${TEST_ROOT}/script.lua"
			CONFIGURATIONS Debug
			DESTINATION ./test/Debug
		)				
		
install	(	FILES "${TEST_ROOT}/script.lua"
			CONFIGURATIONS Release
			DESTINATION ./test/Release
		)						