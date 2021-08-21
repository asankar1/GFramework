REM VS Clang
cmake -B ./build_clang -G "Visual Studio 16 2019" -A x64 -T ClangCL
cmake --build ./build_clang --config Debug --target install

cmake UnitTest -B ./UnitTest/build_clang -G "Visual Studio 16 2019" -A x64 -T ClangCL
cmake --build ./UnitTest/build_clang --config Debug --target install

cmake -B ./build_clang -G "Visual Studio 16 2019" -A x64 -T ClangCL
cmake --build ./build_clang --config Release --target install

cmake UnitTest -B ./UnitTest/build_clang -G "Visual Studio 16 2019" -A x64 -T ClangCL
cmake --build ./UnitTest/build_clang --config Release --target install

REM WSL clang
cmake -B ./build_clang_wsl -G "Unix Makefiles"  -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_clang.cmake
cmake --build ./build_clang_wsl --config Debug --target install

cmake UnitTest -B ./UnitTest/build_clang_wsl -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchain_clang.cmake
cmake --build ./UnitTest/build_clang_wsl --config Debug --target install

cmake -B ./build_clang_wsl -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"  -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_clang.cmake
cmake --build ./build_clang_wsl --config Release --target install

cmake UnitTest -B ./UnitTest/build_clang_wsl -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchain_clang.cmake
cmake --build ./UnitTest/build_clang_wsl --config Release --target install


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


REM Android NDK
cmake -B ./build_android -DCMAKE_BUILD_TYPE=Debug -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_android_arm.cmake .
cmake --build ./build_android --config Debug --target install -j 8

cmake -B ./UnitTest/build_android -DCMAKE_BUILD_TYPE=Debug -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchain_android_arm.cmake ./UnitTest
cmake --build ./UnitTest/build_android --config Debug --target install  -j 8


cmake -B ./build_android -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./cmake/toolchain_android_arm.cmake .
cmake --build ./build_android --config Release --target install -j 8

cmake -B ./UnitTest/build_android -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchain_android_arm.cmake ./UnitTest
cmake --build ./UnitTest/build_android --config Release --target install  -j 8