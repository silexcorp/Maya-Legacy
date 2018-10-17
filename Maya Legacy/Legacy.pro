#-------------------------------------------------
#
# Project created by QtCreator 2016-01-18T17:25:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Legacy
TEMPLATE = app
QT += printsupport

SOURCES += main.cpp\
    s_arbol_avl.C \
    s_listadoble.C \
    s_data.cpp \
    ventana.cpp \
    configuraciones.cpp

HEADERS  += \
    s_arbol_avl.h \
    s_data.h \
    s_listadoble.h \
    ventana.h \
    configuraciones.h

FORMS    += \
    ventana.ui \
    configuraciones.ui
