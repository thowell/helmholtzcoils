#-------------------------------------------------
#
# Project created by QtCreator 2017-08-10T14:52:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = helmholtz_nogui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    helmholtz.cpp \
    field.cpp

HEADERS  += mainwindow.h \
    field.h \
    helmholtz.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/DAQ/release/ -ls626
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/DAQ/debug/ -ls626
else:unix: LIBS += -L$$PWD/DAQ/ -ls626

INCLUDEPATH += $$PWD/DAQ
DEPENDPATH += $$PWD/DAQ

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/DAQ/release/libs626.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/DAQ/debug/libs626.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/DAQ/release/s626.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/DAQ/debug/s626.lib
else:unix: PRE_TARGETDEPS += $$PWD/DAQ/libs626.a
