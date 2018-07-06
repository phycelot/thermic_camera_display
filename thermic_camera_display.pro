#-------------------------------------------------
#
# Project created by QtCreator 2018-06-26T14:21:02
#
#-------------------------------------------------

QT       += core gui widgets
QT += network
INCLUDEPATH += -I /usr/include/opencv2 -Wall
LIBS += -L/usr/lib -l:libopencv_core.so.3.1.0 -l:libopencv_videoio.so.3.1.0 -l:libopencv_imgproc.so.3.1.0 -l:libopencv_highgui.so.3.1.0 -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = thermic_camera_display
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    firstrun.cpp

HEADERS += \
        mainwindow.h \
    firstrun.h

FORMS += \
        mainwindow.ui
