REM cmake -DCMAKE_TOOLCHAIN_FILE=H:\Android\sdk\ndk_bundle\build\cmake\android.toolchain.cmake h:\github\GFramework -G"Unix Makefiles"

REM cmake -DCMAKE_TOOLCHAIN_FILE=H:\Android\sdk\ndk_bundle\build\cmake\android.toolchain.cmake -DANDROID_NDK=H:\Android\sdk\ndk_bundle -DCMAKE_BUILD_TYPE=Debug -DANDROID_ABI="armeabi_v7a with NEON" h:\github\GFramework -G"Unix Makefiles"


REM cmake -DCMAKE_TOOLCHAIN_FILE=H:\github\GFramework\cmake\toolchain_android_arm.cmake -DANDROID_NDK=H:\Android\sdk\ndk_bundle -DCMAKE_BUILD_TYPE=Debug -DANDROID_ABI="armeabi_v7a with NEON" h:\github\GFramework -G"Unix Makefiles"

REM Cygwin_gcc_Debug
cmake -B ./build_cygwin -DCMAKE_BUILD_TYPE=Debug -G"Eclipse CDT4 - Unix Makefiles"
cmake --build ./build_cygwin --config Debug --target install

cmake -B ./UnitTest/build_cygwin -DCMAKE_BUILD_TYPE=Debug -G"Eclipse CDT4 - Unix Makefiles"
cmake --build ./UnitTest/build_cygwin --config Debug --target install

cmake -B ./build_cygwin -DCMAKE_BUILD_TYPE=Release -G"Eclipse CDT4 - Unix Makefiles"
cmake --build ./build_cygwin --config Release --target install

cmake -B ./UnitTest/build_cygwin -DCMAKE_BUILD_TYPE=Release-G"Eclipse CDT4 - Unix Makefiles"
cmake --build ./UnitTest/build_cygwin --config Release --target install
	  
    
REM Cygwin_gcc_Debug
mkdir Build\cygwin_Release
cd Build\cygwin_Release
start "GFramework build: Cygwin_Debug" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_Windows_64_Cygwin.cmake -DCMAKE_BUILD_TYPE=Release ../../
mkdir Test_cygwin_Release
cd Test_cygwin_Release
start "GFramework Test build: Cygwin_Debug" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../../cmake/toolchain_Windows_64_Cygwin.cmake -DCMAKE_BUILD_TYPE=Release ../../../Test
cd ..\..\..\

REM android_arm_Debug
mkdir Build\android_arm_Debug
cd Build\android_arm_Debug
start "GFramework build: android_arm_Debug" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_android_arm.cmake -DCMAKE_BUILD_TYPE=Debug ../../ 
mkdir Test_android_arm_Debug
cd Test_android_arm_Debug
start "GFramework Test build: android_arm_Debug" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../../cmake/toolchain_android_arm.cmake -DCMAKE_BUILD_TYPE=Debug ../../../Test
cd ..\..\..\

REM android_arm_Release
mkdir Build\android_arm_Release
cd Build\android_arm_Release
start "GFramework build: android_arm_Release" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_android_arm.cmake -DCMAKE_BUILD_TYPE=Release ../../ 
mkdir Test_android_arm_Release
cd Test_android_arm_Release
start "GFramework Test build: android_arm_Release" cmd /K cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../../cmake/toolchain_android_arm.cmake -DCMAKE_BUILD_TYPE=Release ../../../Test
cd ..\..\..\

REM VS2015 32
mkdir Build\vs2015
cd Build\vs2015
start "GFramework build: vs2015" cmd /K cmake -G"Visual Studio 14 2015" ../../
mkdir Test_vs2015
cd Test_vs2015
start "GFramework Test build: vs2015" cmd /K cmake -G"Visual Studio 14 2015" ../../../Test
cd ..\..\..\