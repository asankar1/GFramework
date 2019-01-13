#
# To compile for Linux x86_64
#
include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(GFRAMEWORK_TARGET_OS "win")
set(GFRAMEWORK_TARGET_ARCH "x86-64")
set(GFRAMEWORK_TARGET_SOC "")
set(GFRAMEWORK_TARGET_COMPILER "cygwin")
set(GFRAMEWORK_SHARED_LIB_EXT "dll")

set(CMAKE_C_COMPILER H:/cygwin64/bin/gcc.exe)
set(CMAKE_CXX_COMPILER H:/cygwin64/bin/g++.exe)
