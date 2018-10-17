#ifndef S_LISTADOBLE_H
#define S_LISTADOBLE_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

/* LISTA DOBLE ENLAZADA */

struct DATOLISTADOBLE{

    /* Datos de CATEGORIA */
    int   word_id;
    char *word_lexema;
    char *word_n_sig;
    char *word_clase;
    char *word_subclase;
    char *word_sig_esp;
    char *word_def;
    char *word_desc_maya;
    char *word_desc_esp;
    char *word_flexion;
    char *word_voc_esp;
    char *word_ejemplo;
    char *word_trad_ejem;
    char *word_sin;
    char *word_hom;
    char *word_dom_lex;
    char *word_nota;
    char *word_ilust;
    char *word_ref_maya;
    char *word_pres_fuente;
    char *word_pres_pal;
    char *word_fon;
    char *word_fecha;

};

struct NODOLISTADOBLE{
    DATOLISTADOBLE dato_lista_doble;
    struct NODOLISTADOBLE *anterior;
    struct NODOLISTADOBLE *siguiente;
};

struct LISTADOBLE{
    NODOLISTADOBLE *inicio;
    NODOLISTADOBLE *final;
    int tamanio_lista_doble;
};

extern NODOLISTADOBLE *crear_nodo_lista_doble(DATOLISTADOBLE dato);
extern LISTADOBLE *crear_lista_doble();
extern bool lista_doble_vacia(LISTADOBLE *lista);
extern void insertar(LISTADOBLE *lista, DATOLISTADOBLE datos);
extern void agregar_a_lista_doble(LISTADOBLE *lista, DATOLISTADOBLE datos);
extern void eliminar_de_lista_doble(LISTADOBLE *lista, DATOLISTADOBLE dato);
extern bool buscar_dato_lista_doble(LISTADOBLE *lista, DATOLISTADOBLE dato);
extern void mostrar_lista_doble(LISTADOBLE *lista);
extern void mostrar_lista_doble_hash(LISTADOBLE *lista);
extern NODOLISTADOBLE *buscar_nodo_lista_doble(LISTADOBLE *lista, DATOLISTADOBLE dato);

#endif // S_LISTADOBLE_H
