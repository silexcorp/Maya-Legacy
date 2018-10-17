#ifndef S_ARBOL_AVL_H
#define S_ARBOL_AVL_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s_listadoble.h"

using namespace std;

/* ARBOL AVL DE USUARIOS */

struct DATOAVL{
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
    LISTADOBLE *lista_colision;
};

struct NODOAVL{
    DATOAVL dato_avl;
    struct NODOAVL *izq;
    struct NODOAVL *der;
    int fe;
};

struct ARBOLAVLg{
    NODOAVL *raiz;
};

typedef NODOAVL *ARBOLAVL;

extern void insertar_en_avl(ARBOLAVL *raiz, DATOAVL dato);
extern void eliminar_en_avl(ARBOLAVL *raiz, DATOAVL dato);
extern void rotacion_simple(ARBOLAVL *raiz, bool estado);
extern void rotacion_doble(ARBOLAVL *raiz, bool estado);
extern void balancear (ARBOLAVL *raiz);
extern void cambiar_avl(ARBOLAVL *raiz);
extern void factor(ARBOLAVL raiz);
extern void in_orden(ARBOLAVL raiz);
extern bool buscar_nodo_lexema(ARBOLAVL *raiz, DATOAVL dato);
extern bool buscar_nodo_avl(ARBOLAVL *raiz, DATOAVL dato);
extern NODOLISTADOBLE *buscar_nodo(ARBOLAVL *raiz, DATOAVL dato);

#endif // S_ARBOL_AVL_H
