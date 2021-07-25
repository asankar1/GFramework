#-------------------------------------------------
#
# Project created by QtCreator 2019-06-03T23:38:19
#
#-------------------------------------------------

QT       -= core gui

TARGET = Engine
TEMPLATE = lib

DEFINES += GFRAMEWORKTEST_API=__declspec(dllexport)
DEFINES += GFRAMEWORK_API=__declspec(dllimport)
DEFINES += ENGINE_LIBRARY
DEFINES += ENGINESHARED_API=__declspec(dllexport)
DEFINES += GFRAMEWORK_TEMPLATE_API=__declspec(dllexport)

GFRAMEWORK_INSTALL_PATH = $$_PRO_FILE_PWD_/../../output
GFRAMEWORK_SOURCE_PATH = $$_PRO_FILE_PWD_/../..
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

INCLUDEPATH += ..
INCLUDEPATH += $$GFRAMEWORK_SOURCE_PATH/..
INCLUDEPATH += $$GFRAMEWORK_SOURCE_PATH/third_party/boost_1_63_0
INCLUDEPATH += $$GFRAMEWORK_SOURCE_PATH/third_party/glm_0_9_8_3
INCLUDEPATH += $$GFRAMEWORK_SOURCE_PATH/third_party/lua-5.3.4/src

SOURCES += \
        engine.cpp \
        Node.cpp

HEADERS += \
        engine.h \
        Node.h

#find the target os, compiler and version
TARGET_OS = unknown
TARGET_COMPILER_VERSION = unknown
TARGET_ARCH = unknown

mkspec=$$[QMAKE_SPEC]
equals(mkspec, win32-msvc)
{
    MSVC_VER = $$(VisualStudioVersion)
    equals(MSVC_VER, 14.0){
        TARGET_COMPILER_VERSION = vs2015
    }
    equals(MSVC_VER, 15.0){
        TARGET_COMPILER_VERSION = vs2017
    }
    equals(MSVC_VER, 16.0){
        TARGET_COMPILER_VERSION = vs2019
    }
    TARGET_OS = win
    TARGET_ARCH = x64
}

CONFIG(debug, debug|release) {
    LIBS += -L$$GFRAMEWORK_INSTALL_PATH/GFramework/lib/win_64_vs_2015_Debug -lgframework_shared
    BUILD_TYPE=Debug
}
else:CONFIG(release, debug|release) {
    LIBS += -L$$GFRAMEWORK_INSTALL_PATH/GFramework/lib/win_64_vs_2015_Release -lgframework_shared
    BUILD_TYPE=Release
}

#recursively find and install header files
for(header, $$list($$HEADERS)) {
  path = $$_PRO_FILE_PWD_/../output/include/Engine/$$dirname(header)
  pathname = $$replace(path,/,)
  pathname = $$replace(pathname,\.,)
  pathname = $$replace(pathname,:,)
  file = headers_$${pathname}
  eval($${file}.files += $$header)
  eval($${file}.path = $$path)
  INSTALLS *= $${file}
}

target.path += $$_PRO_FILE_PWD_/../output/lib/$${TARGET_OS}_$${TARGET_COMPILER_VERSION}_$${TARGET_ARCH}/$$BUILD_TYPE
INSTALLS += target
