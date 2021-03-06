#-------------------------------------------------
#
# Project created by QtCreator 2013-05-05T10:34:31
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = imedit
TEMPLATE = app
#LIBS +=   -lboost_thread-mt -lboost_system -lboost_filesystem
LIBS += -L $$[QT_INSTALL_LIBS]
MOC_DIR = ./moc
OBJECTS_DIR = ./obj
QMAKE_CXXFLAGS += -Wunused-parameter
INCLUDEPATH += ..
SOURCES += main.cpp\
	imedit.cpp \
        imlineedit.cpp \
	keymap.cpp \
	inputmapper.cpp \
        scripts.cpp \
        mainwindow.cpp

HEADERS  += mainwindow.h \
	keymap.h \
	imedit.h \
        imlineedit.h \
        scripts.h \
	inputmapper.h
