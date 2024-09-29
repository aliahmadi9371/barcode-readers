#-------------------------------------------------
#
# Project created by QtCreator 2019-03-11T13:30:18
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TejaratBank
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp\


HEADERS  += mainwidget.h \


FORMS    += mainwidget.ui


INCLUDEPATH += /usr/lib/arm-linux-gnueabihf
INCLUDEPATH += /usr/include
INCLUDEPATH += /usr/local



win32:CONFIG(release, debug|release): LIBS += -L$$usr/local/lib/release/ -lwiringPi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$usr/local/lib/debug/ -lwiringPi
else:unix: LIBS += -L$$usr/local/lib/ -lwiringPi

INCLUDEPATH += $$usr/local/include
DEPENDPATH += $$usr/local/include

