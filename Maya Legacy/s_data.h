#ifndef S_DATA_H
#define S_DATA_H

#include <qstring.h>

#include <QMainWindow>

#include <QWidget>
#include <QScrollBar>
#include <QObject>
#include <QLabel>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include <QDialog>
#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QImageReader>
#include <QImage>
#include <QFileDialog>
#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QImageReader>

#include "ventana.h"
#include <fstream>      // std::ifstream
#include <sstream>      // std::stringstream, std::stringbuf
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s_listadoble.h"
#include "s_arbol_avl.h"

using namespace std;

class s_data
{
public:
    s_data();
    static ARBOLAVL arbol;
    static Ventana *ventana;
    static bool auto_save, auto_open;
    static char *setting_autor, *setting_correo;
    static char *lista_palabras[];

};

/* Cargar datos */
int convertir(char *dato);
void cargar_datos(); //DATOS (Id, Lexema, Graf, etc);
void generar_reporte_lista_doble(LISTADOBLE *lista, char *name);
void graficar_lista_doble(FILE *archivo, LISTADOBLE *lista, char *name);
void generar_reporte_lista_doble(LISTADOBLE *lista, char *name);
void graficar_lista_doble(FILE *archivo, LISTADOBLE *lista, char *name);
void graficar_avl(NODOAVL *raiz);
void cargar_configuraciones();
/*
* Retorna un vector, de la lectura de un arvhivo */
std::vector <std::string>  choose_file();

#endif // S_DATA_H
