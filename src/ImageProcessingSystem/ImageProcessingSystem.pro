#-------------------------------------------------
#
# Project created by QtCreator 2017-08-09T09:47:22
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageProcessingSystem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
		imagedisplay.cpp \
		mytabwidget.cpp \
		tabcontent.cpp \
                fileoperation.cpp \
                graycommand.cpp \
		imagtranslate.cpp \
                imagegray.cpp \
    negetivecommand.cpp \
    commandlabel.cpp \
    imagecommand.cpp

HEADERS  += mainwindow.h \
		imagedisplay.h \
		mytabwidget.h \
		tabcontent.h \
                fileoperation.h \
                graycommand.h \
		imagtranslate.h \
    imagegray.h \
    negetivecommand.h \
    commandlabel.h \
    imagecommand.h

FORMS    += mainwindow.ui
