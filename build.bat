REM cmake -DCMAKE_TOOLCHAIN_FILE=H:\Android\sdk\ndk-bundle\build\cmake\android.toolchain.cmake h:\github\GFramework -G"Unix Makefiles"

REM cmake -DCMAKE_TOOLCHAIN_FILE=H:\Android\sdk\ndk-bundle\build\cmake\android.toolchain.cmake -DANDROID_NDK=H:\Android\sdk\ndk-bundle -DCMAKE_BUILD_TYPE=Debug -DANDROID_ABI="armeabi-v7a with NEON" h:\github\GFramework -G"Unix Makefiles"


REM cmake -DCMAKE_TOOLCHAIN_FILE=H:\github\GFramework\cmake\toolchain_android_arm.cmake -DANDROID_NDK=H:\Android\sdk\ndk-bundle -DCMAKE_BUILD_TYPE=Debug -DANDROID_ABI="armeabi-v7a with NEON" h:\github\GFramework -G"Unix Makefiles"

REM Cygwin-gcc-Debug
mkdir Build\cygwin-Debug
cd Build\cygwin-Debug
start "GFramework build: Cygwin-Debug" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_Windows_64_Cygwin.cmake -DCMAKE_BUILD_TYPE=Debug ../../ 
mkdir Test-cygwin-Debug
cd Test-cygwin-Debug
start "GFramework Test build: Cygwin-Debug" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../../cmake/toolchain_Windows_64_Cygwin.cmake -DCMAKE_BUILD_TYPE=Debug ../../../Test 
cd ..\..\..\


REM Cygwin-gcc-Debug
mkdir Build\cygwin-Release
cd Build\cygwin-Release
start "GFramework build: Cygwin-Debug" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_Windows_64_Cygwin.cmake -DCMAKE_BUILD_TYPE=Release ../../
mkdir Test-cygwin-Release
cd Test-cygwin-Release
start "GFramework Test build: Cygwin-Debug" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../../cmake/toolchain_Windows_64_Cygwin.cmake -DCMAKE_BUILD_TYPE=Release ../../../Test
cd ..\..\..\

REM android-arm-Debug
mkdir Build\android-arm-Debug
cd Build\android-arm-Debug
start "GFramework build: android-arm-Debug" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_android_arm.cmake -DCMAKE_BUILD_TYPE=Debug ../../ 
mkdir Test-android-arm-Debug
cd Test-android-arm-Debug
start "GFramework Test build: android-arm-Debug" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../../cmake/toolchain_android_arm.cmake -DCMAKE_BUILD_TYPE=Debug ../../../Test
cd ..\..\..\

REM android-arm-Release
mkdir Build\android-arm-Release
cd Build\android-arm-Release
start "GFramework build: android-arm-Release" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_android_arm.cmake -DCMAKE_BUILD_TYPE=Release ../../ 
mkdir Test-android-arm-Release
cd Test-android-arm-Release
start "GFramework Test build: android-arm-Release" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../../cmake/toolchain_android_arm.cmake -DCMAKE_BUILD_TYPE=Release ../../../Test
cd ..\..\..\

REM VS2015 32
mkdir Build\vs2015
cd Build\vs2015
start "GFramework build: vs2015" cmd /K cmake -G"Visual Studio 14 2015" ../../
mkdir Test-vs2015
cd Test-vs2015
start "GFramework Test build: vs2015" cmd /K cmake -G"Visual Studio 14 2015" ../../../Test
cd ..\..\..\