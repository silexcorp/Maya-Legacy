#include "s_arbol_avl.h"
#include "s_data.h"


    /*
     * Crear NODO de avl */
    NODOAVL *crear_nodo_arbol_avl(DATOAVL dato){
        NODOAVL *nuevo = new NODOAVL;
        nuevo->dato_avl = dato;
        nuevo->izq = NULL;
        nuevo->der = NULL;
        nuevo->fe = 0;
        return nuevo;
    }


    /*
     * Insertar en avl */
    void insertar_en_avl(ARBOLAVL *raiz, DATOAVL dato){
         if(dato.word_lexema != NULL){
             dato.word_id = convertir(dato.word_lexema);
             if (*raiz == NULL){
                 dato.lista_colision = crear_lista_doble();
                 *raiz = crear_nodo_arbol_avl(dato);
                 DATOLISTADOBLE dat;
                 dat.word_id =          dato.word_id;
                 dat.word_clase =       dato.word_clase;
                 dat.word_def =         dato.word_def;
                 dat.word_desc_esp =    dato.word_desc_esp;
                 dat.word_desc_maya =   dato.word_desc_maya;
                 dat.word_dom_lex =     dato.word_dom_lex;
                 dat.word_ejemplo =     dato.word_ejemplo;
                 dat.word_fecha =       dato.word_fecha;
                 dat.word_flexion =     dato.word_flexion;
                 dat.word_fon =         dato.word_fon;
                 dat.word_hom =         dato.word_hom;
                 dat.word_ilust =       dato.word_ilust;
                 dat.word_lexema =      dato.word_lexema;
                 dat.word_nota =        dato.word_nota;
                 dat.word_n_sig =       dato.word_n_sig;
                 dat.word_pres_fuente = dato.word_pres_fuente;
                 dat.word_pres_pal =    dato.word_pres_pal;
                 dat.word_ref_maya =    dato.word_ref_maya;
                 dat.word_sig_esp =     dato.word_sig_esp;
                 dat.word_sin =         dato.word_sin;
                 dat.word_subclase =    dato.word_subclase;
                 dat.word_trad_ejem =   dato.word_trad_ejem;
                 dat.word_voc_esp =     dato.word_voc_esp;

                 agregar_a_lista_doble((*raiz)->dato_avl.lista_colision,dat);

             }else if(dato.word_id < (*raiz)->dato_avl.word_id){
                insertar_en_avl(&((*raiz)->izq), dato);
             }else if(dato.word_id > (*raiz)->dato_avl.word_id){
                insertar_en_avl(&((*raiz)->der), dato);
             }else if(dato.word_id == (*raiz)->dato_avl.word_id){
                 DATOLISTADOBLE da; da.word_lexema = dato.word_lexema;
                 bool state = buscar_dato_lista_doble((*raiz)->dato_avl.lista_colision,da);
                 if(state == false){
                     DATOLISTADOBLE dat;
                     dat.word_id =          dato.word_id;
                     dat.word_clase =       dato.word_clase;
                     dat.word_def =         dato.word_def;
                     dat.word_desc_esp =    dato.word_desc_esp;
                     dat.word_desc_maya =   dato.word_desc_maya;
                     dat.word_dom_lex =     dato.word_dom_lex;
                     dat.word_ejemplo =     dato.word_ejemplo;
                     dat.word_fecha =       dato.word_fecha;
                     dat.word_flexion =     dato.word_flexion;
                     dat.word_fon =         dato.word_fon;
                     dat.word_hom =         dato.word_hom;
                     dat.word_ilust =       dato.word_ilust;
                     dat.word_lexema =      dato.word_lexema;
                     dat.word_nota =        dato.word_nota;
                     dat.word_n_sig =       dato.word_n_sig;
                     dat.word_pres_fuente = dato.word_pres_fuente;
                     dat.word_pres_pal =    dato.word_pres_pal;
                     dat.word_ref_maya =    dato.word_ref_maya;
                     dat.word_sig_esp =     dato.word_sig_esp;
                     dat.word_sin =         dato.word_sin;
                     dat.word_subclase =    dato.word_subclase;
                     dat.word_trad_ejem =   dato.word_trad_ejem;
                     dat.word_voc_esp =     dato.word_voc_esp;

                     agregar_a_lista_doble((*raiz)->dato_avl.lista_colision,dat);
                 }
             }
             balancear(raiz);
         }else{
             puts("ERR EN PALABRA");
         }
    }


    /*
     * Cambiar avl */
    void cambiar_avl(ARBOLAVL *raiz){
        NODOAVL *aux, *temporal;temporal = *raiz;
        aux = (*raiz)->izq;
        while(aux->der){
            temporal = aux;
            aux = aux->der;
        }
        (*raiz)->dato_avl = aux->dato_avl;
        if(temporal == (*raiz)){
            temporal->izq = aux->izq;
        }else{
            temporal->der = aux->izq;
        }
        (*raiz) = aux;
    }

    /*
     * Hallar altura */
    int altura(ARBOLAVL raiz){
        int AI, AD;
        if(raiz==NULL)
            return 0;
        else{
            AI = altura(raiz->izq);
            AD = altura(raiz->der);
            if(AI>AD){
                return AI+1;
            }else{
                return AD+1;
            }
        }
    }

    /*
     * Buscar el factor de equilibro */
    void factor(ARBOLAVL raiz){
        if (raiz != NULL) {
           factor(raiz->izq);
           int alt_izq,alt_der,fe;

           alt_izq = altura(raiz->izq);
           alt_der = altura(raiz->der);
           fe = alt_der - alt_izq;

           raiz->fe = fe;
           factor(raiz->der);
        }
    }

    /*
     * Rotaciones simples II DD */
    void rotacion_simple(ARBOLAVL *raiz, bool izq){
        ARBOLAVL temporal;
         if (izq){
             temporal = (*raiz)->izq;
             (*raiz)->izq = temporal->der;
             temporal->der = *raiz;
         }else{
             temporal = (*raiz)->der;
             (*raiz)->der = temporal->izq;
             temporal->izq = *raiz;
         }*raiz = temporal;
    }

    /*
     * Rotaciones dobles ID DI */
    void rotacion_doble(ARBOLAVL *raiz, bool izq){
        if (izq){
            rotacion_simple(&(*raiz)->izq, false);
            rotacion_simple(raiz, true);
        }else{
            rotacion_simple(&(*raiz)->der, true);
            rotacion_simple(raiz, false);
        }
     }

    /*
     * Balancear avl */
    void balancear (ARBOLAVL *raiz){
      if((*raiz)!=NULL){
        if (altura((*raiz)->izq) - altura((*raiz)->der) == 2){
            if (altura ((*raiz)->izq->izq) >= altura ((*raiz)->izq->der)){
                rotacion_simple(raiz, true);
            }else{
                rotacion_doble(raiz, true);}
          }else if (altura((*raiz)->der) - altura((*raiz)->izq) == 2){
            if (altura ((*raiz)->der->der) >= altura ((*raiz)->der->izq)){
                rotacion_simple(raiz, false);
            }else{
                rotacion_doble(raiz, false);}
          }
      }
    }

    /*
     * Buscar nodo en avl */
    bool buscar_nodo_avl(ARBOLAVL *raiz, DATOAVL dato){
        bool eleccion = false;
        if((*raiz) != NULL){
            if(dato.word_id < (*raiz)->dato_avl.word_id){
                eleccion = buscar_nodo_avl(&((*raiz)->izq),dato);
            }else if(dato.word_id > (*raiz)->dato_avl.word_id){
                eleccion = buscar_nodo_avl(&((*raiz)->der), dato);
            }else if(dato.word_id == (*raiz)->dato_avl.word_id){ return true; }
        }else{ return eleccion; }
    }




    /*
     * Buscar nodo en avl */
    bool buscar_nodo_lexema(ARBOLAVL *raiz, DATOAVL dato){
        if(dato.word_lexema != NULL){
            dato.word_id = convertir(dato.word_lexema);
            if(*raiz != NULL){
                if(dato.word_id < (*raiz)->dato_avl.word_id){
                    return buscar_nodo_lexema(&((*raiz)->izq), dato);
                }else if(dato.word_id > (*raiz)->dato_avl.word_id){
                    return buscar_nodo_lexema(&((*raiz)->der), dato);
                }else if(dato.word_id == (*raiz)->dato_avl.word_id){
                    if(strcmp(dato.word_lexema, (*raiz)->dato_avl.word_lexema) != 0){
                        DATOLISTADOBLE da; da.word_lexema = dato.word_lexema;
                        return buscar_dato_lista_doble((*raiz)->dato_avl.lista_colision,da);
                    }else{
                        return true;
                    }
                }
            }
        }else{
            return false;
        }
    }
    /* Continuar con la insercion y bussqueda en el avl */


    /*
     * Eliminar en avl */
    void eliminar_en_avl(ARBOLAVL *raiz, DATOAVL dato){
        if((*raiz) != NULL){
            dato.word_id = convertir(dato.word_lexema);
            if(dato.word_id < (*raiz)->dato_avl.word_id){
                eliminar_en_avl(&((*raiz)->izq),dato);
            }else if(dato.word_id > (*raiz)->dato_avl.word_id){
                eliminar_en_avl(&((*raiz)->der), dato);
            }else if(dato.word_id == (*raiz)->dato_avl.word_id){
                puts("      Supongo que lo encontro XD");
                NODOAVL *aux; aux = (*raiz);
                if(aux->dato_avl.lista_colision == NULL){
                    if(aux->izq == NULL){
                        (*raiz) = aux->der;
                    }else if(aux->der == NULL){
                        (*raiz) = aux->izq;
                    }else{
                        cambiar_avl(&aux);
                    }
                    delete(aux);
                    balancear(raiz);
                }else{
                    DATOLISTADOBLE dat;
                    dat.word_lexema = new char [sizeof(dato.word_lexema)+1];
                    strcpy(dat.word_lexema, dato.word_lexema);
                    //dat.word_lexema = dato.word_lexema;
                    eliminar_de_lista_doble(aux->dato_avl.lista_colision,dat);
                }
            }
        }
    }




    //Busqueda de un Nodo
    NODOLISTADOBLE *buscar_nodo(ARBOLAVL *raiz, DATOAVL dato){
        if((*raiz) != NULL){
            dato.word_id = convertir(dato.word_lexema);
            if(dato.word_id < (*raiz)->dato_avl.word_id){
                buscar_nodo(&((*raiz)->izq),dato);
            }else if(dato.word_id > (*raiz)->dato_avl.word_id){
                buscar_nodo(&((*raiz)->der), dato);
            }else{
                //puts("");
                DATOLISTADOBLE da;
                da.word_lexema = dato.word_lexema;
                da.word_id =     dato.word_id;
                return buscar_nodo_lista_doble((*raiz)->dato_avl.lista_colision,da);
            }
        }
    }


    //Eliminar
    void eliminar_nodo(ARBOLAVL *raiz, DATOAVL dato){
        if(!(*raiz)){
            //Nodo no encontrado
        }else{

            if(dato.word_id < (*raiz)->dato_avl.word_id){
                eliminar_nodo(&((*raiz)->izq), dato);

            }else if(dato.word_id > (*raiz)->dato_avl.word_id){
                eliminar_nodo(&((*raiz)->der), dato);

            }else{
                //Cambio en los nodos
                NODOAVL *aux;
                aux = (*raiz);
                if(aux->izq == NULL){
                    (*raiz) = aux->der;
                }else if(aux->der == NULL){
                    (*raiz) = aux->izq;
                }else{
                    cambiar_avl(&aux);
                }
                delete(aux);


                balancear(raiz);

            }

        }
    }


    /*
     * Mostrar datos inorden */
    void in_orden(ARBOLAVL raiz){
        if(raiz != NULL){
            in_orden(raiz->izq);
                printf("ID: %d\n",raiz->dato_avl.word_id);
                if(raiz->dato_avl.lista_colision != NULL){
                    mostrar_lista_doble(raiz->dato_avl.lista_colision);
                }
            in_orden(raiz->der);
        }
    }

