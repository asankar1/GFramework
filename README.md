# GFramework

The idea of GFramework is to develop a 'Generic framework' to create any application that supports and test the following:

  * Variant
  * Reflection
  * Serialization with versioning
  * Lua script
  * Doxygen generation

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Platforms supported

This currently supports WIndows OS and built using Visual C++ 2015. However, it will not be difficult to port it to other build tools and platforms, if the prerequisites are met.

### Prerequisites

To build this project you need
  * C++14 compatible compiler
  * Boost header only library files 1.63.0
  * glm math library 0.9.8
  * Lua 5.3.4
  * Doxygen generator

## Build status
[![Badge for Visual Studio 2015 and 2017](https://img.shields.io/github/workflow/status/asankar1/GFramework/windows_vs_cmake/integration?label=Visual%20Studio%202015%20and%202017&style=plastic)](https://github.com/asankar1/GFramework/actions/workflows/windows_vs_cmake.yml)
[![Badge forUbuntu gcc](https://img.shields.io/github/workflow/status/asankar1/GFramework/ubuntu_gcc_cmake/integration?label=Ubuntu%20gcc&style=plastic)](https://github.com/asankar1/GFramework/actions/workflows/ubuntu_gcc_cmake.yml)
[![Badge for Android NDK](https://img.shields.io/github/workflow/status/asankar1/GFramework/ubuntu_android_ndk/integration?label=Android%20NDK&style=plastic)](https://github.com/asankar1/GFramework/actions/workflows/ubuntu_android_ndk.yml)

## Unit test reports
[Windows_VS2015_Debug](https://github.com/asankar1/GFramework/blob/master/UnitTest/Reports/Windows_VS2015_Debug_Report.xml)  
[Windows_VS2015_RelWithDebInfo](https://github.com/asankar1/GFramework/blob/master/UnitTest/Reports/Windows_VS2015_RelWithDebInfo_Report.xml)  
[Windows_VS2015_Release](https://github.com/asankar1/GFramework/blob/master/UnitTest/Reports/Windows_VS2015_Debug_Release.xml)  
[Windows_VS2017_Debug](https://github.com/asankar1/GFramework/blob/master/UnitTest/Reports/Windows_VS2017_Debug_Report.xml)  
[Windows_VS2017_RelWithDebInfo](https://github.com/asankar1/GFramework/blob/master/UnitTest/Reports/Windows_VS2017_RelWithDebInfo_Report.xml)  
[Windows_VS2017_Release](https://github.com/asankar1/GFramework/blob/master/UnitTest/Reports/Windows_VS2017_Debug_Release.xml)  

## Coverage reports
[![Code Coverage](https://codecov.io/gh/asankar1/GFramework/branch/integration/graph/badge.svg?token=C3FOMEPHJ3)](https://codecov.io/gh/asankar1/GFramework)
