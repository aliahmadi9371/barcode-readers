#-------------------------------------------------
#
# Project created by QtCreator 2017-12-18T13:55:01
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ali2
TEMPLATE = app


SOURCES += main.cpp\
    mainform.cpp \
    filerotationlogsink.cpp

HEADERS  += \
    mainform.h \
    filerotationlogsink.h

FORMS    += \
    mainform.ui



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/local/lib/release/ -lwiringPi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/local/lib/debug/ -lwiringPi
else:unix: LIBS += -L$$PWD/../../../usr/local/lib/ -lwiringPi

INCLUDEPATH += $$PWD/../../../usr/local/include
DEPENDPATH += $$PWD/../../../usr/local/include


