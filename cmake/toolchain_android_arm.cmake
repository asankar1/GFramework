#
# To cross compile from WIN32 to android, using android NDK
#


set(CMAKE_SYSTEM_NAME "Android")
set(CMAKE_SYSTEM_PROCESSOR "armv7-a")

set(ANDROID_ABI "armeabi-v7a with NEON")
set(ANDROID_NDK "H:/Android/sdk/ndk-bundle")
set(ANDROID_NATIVE_API_LEVEL 28)

set(CMAKE_C_COMPILER "H:/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/windows-x86_64/bin/clang.exe")
set(CMAKE_CXX_COMPILER "H:/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe")

set(GFRAMEWORK_TARGET_OS "android")
set(GFRAMEWORK_TARGET_COMPILER "gcc")
set(GFRAMEWORK_TARGET_ARCH "armv7")
set(GFRAMEWORK_TARGET_SOC "")
set(GFRAMEWORK_SHARED_LIB_EXT "so")
set(GFRAMEWORK_IMPORT_API )
set(GFRAMEWORK_EXPORT_API )

