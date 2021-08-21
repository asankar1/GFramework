#
# To cross compile for android, using android NDK
#


set(CMAKE_SYSTEM_NAME "Android")
set(CMAKE_SYSTEM_PROCESSOR "armv7-a")
set(CMAKE_SYSTEM_VERSION 28)
set(CMAKE_ANDROID_ARCH_ABI armeabi-v7a)
string(REPLACE "\\" "/" CMAKE_ANDROID_NDK $ENV{ANDROID_NDK_LATEST_HOME})
message(STATUS 	"CMAKE_ANDROID_NDK=${CMAKE_ANDROID_NDK}")
set(CMAKE_ANDROID_STL_TYPE c++_static)

#set(ANDROID_ABI "armeabi-v7a with NEON")
string(REPLACE "\\" "/" ANDROID_NDK $ENV{ANDROID_NDK_LATEST_HOME})
#set(ANDROID_NATIVE_API_LEVEL 28)
#set(ANDROID_TARGET_TRIPLE armv7a-linux-androideabi${ANDROID_NATIVE_API_LEVEL})

set(CMAKE_C_COMPILER "${ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/bin/clang.exe")
set(CMAKE_CXX_COMPILER "${ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe")
#set(CMAKE_C_FLAGS "-target ${ANDROID_TARGET_TRIPLE}")
#set(CMAKE_CXX_FLAGS "-target ${ANDROID_TARGET_TRIPLE}")

set(GFRAMEWORK_TARGET_OS "android")
set(GFRAMEWORK_TARGET_COMPILER "clang")
set(GFRAMEWORK_TARGET_ARCH "armv7a")
set(GFRAMEWORK_TARGET_SOC "")
set(GFRAMEWORK_SHARED_LIB_EXT "so")
set(GFRAMEWORK_IMPORT_API )
set(GFRAMEWORK_EXPORT_API )

