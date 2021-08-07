REM cmake -DCMAKE_TOOLCHAIN_FILE=H:\Android\sdk\ndk_bundle\build\cmake\android.toolchain.cmake h:\github\GFramework -G"Unix Makefiles"

REM cmake -DCMAKE_TOOLCHAIN_FILE=H:\Android\sdk\ndk_bundle\build\cmake\android.toolchain.cmake -DANDROID_NDK=H:\Android\sdk\ndk_bundle -DCMAKE_BUILD_TYPE=Debug -DANDROID_ABI="armeabi_v7a with NEON" h:\github\GFramework -G"Unix Makefiles"


REM cmake -DCMAKE_TOOLCHAIN_FILE=H:\github\GFramework\cmake\toolchain_android_arm.cmake -DANDROID_NDK=H:\Android\sdk\ndk_bundle -DCMAKE_BUILD_TYPE=Debug -DANDROID_ABI="armeabi_v7a with NEON" h:\github\GFramework -G"Unix Makefiles"

REM MSVC
cmake -B ./build_vs2015 -G "Visual Studio 16 2019" -A x64 -T v140
cmake --build ./build_vs2015 --config Debug --target install
cmake --build ./build_vs2015 --config Release --target install
  
cmake UnitTest -B ./UnitTest/build_vs2015 -G "Visual Studio 16 2019" -A x64 -T v140
cmake --build ./UnitTest/build_vs2015 --config Debug --target install
cmake --build ./UnitTest/build_vs2015 --config Release --target install

REM Cygwin_gcc
cmake -B ./build_cygwin -DCMAKE_BUILD_TYPE=Debug -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_Windows_64_Cygwin.cmake .
cmake --build ./build_cygwin --config Debug --target install -j 8

cmake -B ./UnitTest/build_cygwin -DCMAKE_BUILD_TYPE=Debug -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchain_Windows_64_Cygwin.cmake ./UnitTest
cmake --build ./UnitTest/build_cygwin --config Debug --target install  -j 8

cmake -B ./build_cygwin -DCMAKE_BUILD_TYPE=Release -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_Windows_64_Cygwin.cmake .
cmake --build ./build_cygwin --config Release --target install

cmake -B ./UnitTest/build_cygwin -DCMAKE_BUILD_TYPE=Release -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchain_Windows_64_Cygwin.cmake ./UnitTest
cmake --build ./UnitTest/build_cygwin --config Release --target install


REM WSL gcc
cmake -B ./build_wsl -DCMAKE_BUILD_TYPE=Debug -G"Unix Makefiles" .
cmake --build ./build_wsl --config Debug --target install -j 8

cmake -B ./UnitTest/build_wsl -DCMAKE_BUILD_TYPE=Debug -G"Unix Makefiles" ./UnitTest
cmake --build ./UnitTest/build_wsl --config Debug --target install  -j 8

cmake -B ./build_wsl -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" .
cmake --build ./build_wsl --config Release --target install

cmake -B ./UnitTest/build_wsl -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" ./UnitTest
cmake --build ./UnitTest/build_wsl --config Release --target install


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

