#-------------------------------------------------
#
# Project created by QtCreator 2019-05-11T12:48:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GFrameworkTest
TEMPLATE = app

GFRAMEWORK_INSTALL_PATH = $$_PRO_FILE_PWD_/../../output
GFRAMEWORK_SOURCE_PATH = $$_PRO_FILE_PWD_/../..

GEDITOR_INSTALL_PATH = $$_PRO_FILE_PWD_/../../output
GEDITOR_SOURCE_PATH = $$_PRO_FILE_PWD_/../..

ENGINE_INSTALL_PATH = $$_PRO_FILE_PWD_/../output
ENGINE_SOURCE_PATH = $$_PRO_FILE_PWD_/..

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += GFRAMEWORK_API=__declspec(dllimport)
DEFINES += GFRAMEWORKTEST_API=__declspec(dllimport)
DEFINES += ENGINESHARED_API=__declspec(dllimport)
DEFINES += GFRAMEWORK_TEMPLATE_API=__declspec(dllimport)

CONFIG += c++11

INCLUDEPATH += $$_PRO_FILE_PWD_/src
INCLUDEPATH += $$ENGINE_INSTALL_PATH/include
INCLUDEPATH += $$GEDITOR_SOURCE_PATH
INCLUDEPATH += $$GFRAMEWORK_SOURCE_PATH/..
INCLUDEPATH += $$GFRAMEWORK_SOURCE_PATH/third_party/boost_1_63_0
INCLUDEPATH += $$GFRAMEWORK_SOURCE_PATH/third_party/glm_0_9_8_3
INCLUDEPATH += $$GFRAMEWORK_SOURCE_PATH/third_party/lua-5.3.4/src

SOURCES += \
        src/main.cpp \
        src/testapplication.cpp \
        src/testproject.cpp

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
DEPENDPATH += $$PWD/../../GFrameworkEditor

CONFIG(debug, debug|release) {
    BUILD_TYPE=Debug
    LIBS += -L$$GEDITOR_INSTALL_PATH/GEditor/lib/$${TARGET_OS}_$${TARGET_COMPILER_VERSION}_$${TARGET_ARCH}/$$BUILD_TYPE -lGEditor
    LIBS += -L$$GFRAMEWORK_INSTALL_PATH/GFramework/lib/win_64_vs_2015_$$BUILD_TYPE -lgframework_shared
    LIBS += -L$$ENGINE_INSTALL_PATH/lib/$${TARGET_OS}_$${TARGET_COMPILER_VERSION}_$${TARGET_ARCH}/$$BUILD_TYPE/ -lEngine
}
else:CONFIG(release, debug|release) {
    BUILD_TYPE=Release
    LIBS += -L$$GEDITOR_INSTALL_PATH/GEditor/lib/$${TARGET_OS}_$${TARGET_COMPILER_VERSION}_$${TARGET_ARCH}/$$BUILD_TYPE -lGEditor
    LIBS += -L$$GFRAMEWORK_INSTALL_PATH/GFramework/lib/win_64_vs_2015_$$BUILD_TYPE -lgframework_shared
    LIBS += -L$$ENGINE_INSTALL_PATH/lib/$${TARGET_OS}_$${TARGET_COMPILER_VERSION}_$${TARGET_ARCH}/$$BUILD_TYPE/ -lEngine
}

# Must use $$OUT_PWD as target install path, because this is the default working directory
target_depends.path += $$OUT_PWD
target_depends.files += $$ENGINE_INSTALL_PATH/lib/$${TARGET_OS}_$${TARGET_COMPILER_VERSION}_$${TARGET_ARCH}/$$BUILD_TYPE/Engine.dll
target_depends.files += $$GEDITOR_INSTALL_PATH/GEditor/lib/$${TARGET_OS}_$${TARGET_COMPILER_VERSION}_$${TARGET_ARCH}/$$BUILD_TYPE/GEditor.dll
target_depends.files += $$GFRAMEWORK_INSTALL_PATH/GFramework/bin/win_64_vs_2015_$${BUILD_TYPE}/gframework_shared.dll

target.path = $$OUT_PWD
INSTALLS += target
INSTALLS += target_depends
