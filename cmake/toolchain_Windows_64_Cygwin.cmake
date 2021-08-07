#
# To compile for Cygwin x86_64
#
include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(GFRAMEWORK_TARGET_OS "win")
set(GFRAMEWORK_TARGET_ARCH "x86-64")
set(GFRAMEWORK_TARGET_SOC "")
set(GFRAMEWORK_TARGET_COMPILER "cygwin")
set(GFRAMEWORK_SHARED_LIB_EXT "")

set(CMAKE_C_COMPILER gcc.exe)
set(CMAKE_CXX_COMPILER g++.exe)

set(GFRAMEWORK_IMPORT_API __declspec\(dllimport\))
set(GFRAMEWORK_EXPORT_API __declspec\(dllexport\))