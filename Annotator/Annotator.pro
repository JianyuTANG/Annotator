#-------------------------------------------------
#
# Project created by QtCreator 2019-08-26T22:26:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Annotator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    image2d.cpp \
    paintboard.cpp \
    annotation.cpp \
    annotationlist.cpp \
    annotationitem.cpp \
    image3d.cpp \
    paintboard3d.cpp \
    interface3d.cpp

HEADERS += \
        mainwindow.h \
    image2d.h \
    paintboard.h \
    annotation.h \
    annotationlist.h \
    annotationitem.h \
    image3d.h \
    paintboard3d.h \
    interface3d.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += D:\opencv\build\include

LIBS += D:\opencv-build\bin\libopencv_core411.dll
LIBS += D:\opencv-build\bin\libopencv_highgui411.dll
LIBS += D:\opencv-build\bin\libopencv_imgcodecs411.dll
LIBS += D:\opencv-build\bin\libopencv_imgproc411.dll
LIBS += D:\opencv-build\bin\libopencv_features2d411.dll
LIBS += D:\opencv-build\bin\libopencv_calib3d411.dll

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc
