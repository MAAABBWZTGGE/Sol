#-------------------------------------------------
#
# Project created by QtCreator 2013-10-08T23:52:56
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = PSC-Sol
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    data.cpp \
    logger.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    data.h \
    logger.h

FORMS    += mainwindow.ui


cache()
