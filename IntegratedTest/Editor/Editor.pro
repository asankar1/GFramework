#-------------------------------------------------
#
# Project created by QtCreator 2019-05-11T12:48:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GFrameworkTest
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += GFRAMEWORK_API=__declspec(dllimport)
DEFINES += GFRAMEWORKTEST_API=__declspec(dllimport)
DEFINES += ENGINESHARED_API=__declspec(dllimport)

CONFIG += c++11

INCLUDEPATH += ..
INCLUDEPATH += ../..
INCLUDEPATH += ../../third_party/boost_1_63_0
INCLUDEPATH += ../../third_party/glm_0_9_8_3
INCLUDEPATH += ../../third_party/lua-5.3.4/src

SOURCES += \
        src/main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DEPENDPATH += $$PWD/../../GFrameworkEditor

win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../output/GFramework/lib/win_64_vs_2015_Release/ -lGEditor
else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../output/GFramework/lib/win_64_vs_2015_Debug/ -lGEditord
else:unix: LIBS += -L$$_PRO_FILE_PWD_/../../output/GFramework/lib/linux/ -lGEditor

win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../output/GFramework/lib/win_64_vs_2015_Release -lEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../output/GFramework/lib/win_64_vs_2015_Debug -lEngine
else:unix: LIBS += -L$$_PRO_FILE_PWD_/../../output/GFramework/lib/linux -lEngine

win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../output/GFramework/lib/win_64_vs_2015_Release -lgframework_shared
else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/../../output/GFramework/lib/win_64_vs_2015_Debug -lgframework_shared
else:unix: LIBS += -L$$_PRO_FILE_PWD_/../../output/GFramework/lib/linux -lGFrameworkShared

INCLUDEPATH += $$_PRO_FILE_PWD_/../Engine
DEPENDPATH += $$_PRO_FILE_PWD_/../Engine

Debug:DESTDIR = $$_PRO_FILE_PWD_/../../output/GFramework/bin/win_64_vs_2015_Debug
Release:DESTDIR = $$_PRO_FILE_PWD_/../../output/GFramework/bin/win_64_vs_2015_Release
