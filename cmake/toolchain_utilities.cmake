#
# To set additional options based on the toolchain used
#

set(GFRAMEWORK_TARGET_OS "UnkownOS")
set(GFRAMEWORK_TARGET_ARCH "UnkownArch")
set(GFRAMEWORK_TARGET_SOC "")
set(GFRAMEWORK_TARGET_COMPILER "UnknownCompiler")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
set(GFRAMEWORK_TARGET_OS "win")
else()
set(GFRAMEWORK_TARGET_OS ${CMAKE_SYSTEM_NAME})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

if(${CMAKE_SIZEOF_VOID_P} MATCHES "4")
	set(GFRAMEWORK_TARGET_ARCH "32")
elseif(${CMAKE_SIZEOF_VOID_P} MATCHES "8")
	set(GFRAMEWORK_TARGET_ARCH "64")
else()
	message(FATAL_ERROR "Unknown Target processor!")
endif(${CMAKE_SIZEOF_VOID_P} MATCHES "4")	

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(GFRAMEWORK_TARGET_COMPILER "vs_2015")
	set(GFRAMEWORK_BUILD_TYPE "$(Configuration)")
else()
	set(GFRAMEWORK_TARGET_COMPILER ${CMAKE_CXX_COMPILER_ID})
	if("${CMAKE_BUILD_TYPE} " STREQUAL " ")
		message(WARNING "CMAKE_BUILD_TYPE not specified, assuming Debug!")
		set(CMAKE_BUILD_TYPE "Debug")
		set(GFRAMEWORK_BUILD_TYPE "Debug")
	else()
		set(GFRAMEWORK_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
	endif("${CMAKE_BUILD_TYPE} " STREQUAL " ")	
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(GFRAMEWORK_IMPORT_API __declspec\(dllimport\))
	set(GFRAMEWORK_EXPORT_API __declspec\(dllexport\))
	set(GFRAMEWORK_SHARED_LIB_EXT "dll")
else()	
	set(GFRAMEWORK_IMPORT_API )
	set(GFRAMEWORK_EXPORT_API )
	set(GFRAMEWORK_SHARED_LIB_EXT "so")	
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")	
	# if(${CMAKE_SIZEOF_VOID_P} MATCHES "4")
		# set(GFRAMEWORK_TARGET_OS "linux")
		# set(GFRAMEWORK_TARGET_ARCH "x86")
		# set(GFRAMEWORK_TARGET_SOC "")
	# elseif(${CMAKE_SIZEOF_VOID_P} MATCHES "8")
		# set(GFRAMEWORK_TARGET_OS "linux")
		# set(GFRAMEWORK_TARGET_ARCH "x86_64")
		# set(GFRAMEWORK_TARGET_SOC "")
	# else()
		# message(FATAL_ERROR "Unknown Target processor for Linux!")
	# endif(${CMAKE_SIZEOF_VOID_P} MATCHES "4")
	# set(GFRAMEWORK_TARGET_COMPILER "gcc")
	# set(GFRAMEWORK_IMPORT_API )
	# set(GFRAMEWORK_EXPORT_API )		
# else()
	# set(GFRAMEWORK_IMPORT_API )
	# set(GFRAMEWORK_EXPORT_API )	
# endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set(GFRAMEWORK_TARGET_OS_ARCH ${GFRAMEWORK_TARGET_OS}_${GFRAMEWORK_TARGET_ARCH})

if(GFRAMEWORK_TARGET_SOC STREQUAL "")
	set(GFRAMEWORK_TARGET_OS_ARCH_SOC ${GFRAMEWORK_TARGET_OS_ARCH})
else()
	set(GFRAMEWORK_TARGET_OS_ARCH_SOC ${GFRAMEWORK_TARGET_OS_ARCH}_${GFRAMEWORK_TARGET_SOC})	
endif(GFRAMEWORK_TARGET_SOC STREQUAL "")

if(GFRAMEWORK_TARGET_COMPILER STREQUAL "")
	set(GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER ${GFRAMEWORK_TARGET_OS_ARCH})
else()
	set(GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER ${GFRAMEWORK_TARGET_OS_ARCH}_${GFRAMEWORK_TARGET_COMPILER})	
endif(GFRAMEWORK_TARGET_COMPILER STREQUAL "")

set(GFRAMEWORK_API )



	
message(STATUS "GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER: " ${GFRAMEWORK_TARGET_OS_ARCH_SOC_COMPILER})
message(STATUS "GFRAMEWORK_BUILD_TYPE:${GFRAMEWORK_BUILD_TYPE}")

macro(set_output_paths library_name)
	if(${GFRAMEWORK_TARGET_OS} MATCHES "win")
	set_target_properties(${library_name}_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug" )
	set_target_properties(${library_name}_static PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug" )
	set_target_properties(${library_name}_static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug" )
	set_target_properties(${library_name}_shared PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug" )
	set_target_properties(${library_name}_shared PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug" )
	set_target_properties(${library_name}_shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug" )
	
	target_compile_definitions(${library_name}_static PUBLIC GFRAMEWORK_API=)	
	
	set_target_properties(${library_name}_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release" )
	set_target_properties(${library_name}_static PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release" )
	set_target_properties(${library_name}_static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release" )
	set_target_properties(${library_name}_shared PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release" )
	set_target_properties(${library_name}_shared PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release" )
	set_target_properties(${library_name}_shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release" )
	
	target_compile_definitions(${library_name}_shared PUBLIC GFRAMEWORK_API=${GFRAMEWORK_EXPORT_API})	
	else()
		#message(FATAL_ERROR "Macro 'set_output_paths' is not supported for the OS " ${GFRAMEWORK_TARGET_OS})
		target_compile_definitions(${library_name}_static PUBLIC GFRAMEWORK_API=)
		target_compile_definitions(${library_name}_shared PUBLIC GFRAMEWORK_API=)
	endif(${GFRAMEWORK_TARGET_OS} MATCHES "win")
endmacro()
