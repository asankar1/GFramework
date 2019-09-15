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
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

INCLUDEPATH = $$OUT_PWD/../../../..
INCLUDEPATH += $$_PRO_FILE_PWD_/../../
INCLUDEPATH += $$_PRO_FILE_PWD_/../../third_party/boost_1_63_0
INCLUDEPATH += $$_PRO_FILE_PWD_/../../third_party/glm_0_9_8_3
INCLUDEPATH += $$_PRO_FILE_PWD_/../../third_party/lua-5.3.4/src

SOURCES += \
        engine.cpp \
        Node.cpp

HEADERS += \
        engine.h \
        Node.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../output/GFramework/lib/win_64_vs_2015_Release -lgframework_shared
else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../output/GFramework/lib/win_64_vs_2015_Debug -lgframework_shared
else:unix: LIBS += -L$$_PRO_FILE_PWD_/../../output/GFramework/lib/linux -lGFrameworkShared

Debug:DESTDIR = $$_PRO_FILE_PWD_/../../output/GFramework/lib/win_64_vs_2015_Debug
Release:DESTDIR = $$_PRO_FILE_PWD_/../../output/GFramework/lib/win_64_vs_2015_Release
Debug:DLLDESTDIR = $$_PRO_FILE_PWD_/../../output/GFramework/bin/win_64_vs_2015_Debug
Release:DLLDESTDIR = $$_PRO_FILE_PWD_/../../output/GFramework/bin/win_64_vs_2015_Release
