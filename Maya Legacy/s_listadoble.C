#include "s_listadoble.h"
#include "s_data.h"

/*
 * Crear NODO de lista doble */
NODOLISTADOBLE *crear_nodo_lista_doble(DATOLISTADOBLE dato){
    NODOLISTADOBLE *nuevo = new NODOLISTADOBLE;
    nuevo->dato_lista_doble = dato;
    nuevo->anterior = NULL;
    nuevo->siguiente = NULL;
    return nuevo;
}


/*
 * Crear lista doble */
LISTADOBLE *crear_lista_doble(){
    LISTADOBLE *lista = new LISTADOBLE();
    lista->final = NULL;
    lista->inicio = NULL;
    lista->tamanio_lista_doble = 0;
    return lista;
}


/*
 * Verificar si lista doble esta vacia */
bool lista_doble_vacia(LISTADOBLE *lista){
    if(lista->inicio == NULL && lista->final == NULL){
        return true;
    }else{
        return false;
    }
}

/*
 * Buscar categoría de NOTICIA en lista doble */
bool buscar_dato_lista_doble(LISTADOBLE *lista, DATOLISTADOBLE dato){
    NODOLISTADOBLE *aux = lista->inicio; bool hallado = false;
    if(!lista_doble_vacia(lista)){
        while (aux != NULL) {//strcmp
            if(strcmp(aux->dato_lista_doble.word_lexema, dato.word_lexema) == 0){
                hallado = true;break;
            }
            aux = aux->siguiente;
        }
    }

    return hallado;
}

/*
 * Buscar categoría de NOTICIA en lista doble */
NODOLISTADOBLE *buscar_nodo_lista_doble(LISTADOBLE *lista, DATOLISTADOBLE dato){
    NODOLISTADOBLE *aux = lista->inicio; NODOLISTADOBLE *hallado = NULL;
    if(!lista_doble_vacia(lista)){
        while (aux != NULL) {
            if(strcmp(aux->dato_lista_doble.word_lexema, dato.word_lexema) == 0){
                hallado = aux;break;//puts("MMMMM SUPONGO");
            }
            aux = aux->siguiente;
        }
    }
    return hallado;
}

/*
 * Agregar nuevo elemento al final lista doble */
void agregar_a_lista_doble(LISTADOBLE *lista, DATOLISTADOBLE datos){
    bool insertar = buscar_dato_lista_doble(lista,datos);
    if(insertar != true){
        //cout << "INSERTANDO: "<< datos.word_lexema << endl;
        NODOLISTADOBLE *nuevo = crear_nodo_lista_doble(datos);
        if(lista_doble_vacia(lista)){
            lista->inicio = nuevo;
            lista->final = nuevo;
            lista->tamanio_lista_doble++;
        }else{
            nuevo->anterior = lista->final;
            lista->final->siguiente = nuevo;
            lista->final = nuevo;
            lista->tamanio_lista_doble++;
        }
    }else{
        cout << "NO SE INSERTO: "<< datos.word_lexema << endl;
    }
}

/*
 * Insertar nuevo elemento al final lista doble */
void insertar(LISTADOBLE *lista, DATOLISTADOBLE datos){
    bool insertar = buscar_dato_lista_doble(lista,datos);
    if(insertar != true){
        //cout << "INSERTANDO: "<< datos.word_id << endl;
        NODOLISTADOBLE *aux, *aux2, *nuevo = crear_nodo_lista_doble(datos);
        if(datos.word_lexema != NULL){
            datos.word_id = convertir(datos.word_lexema);

            if(lista_doble_vacia(lista)){
                lista->inicio = nuevo;
                lista->final = nuevo;
                lista->tamanio_lista_doble++;
            }else{
                aux = lista->inicio;
                while(aux != NULL){
                    aux2 = aux->siguiente;
                    if(nuevo->dato_lista_doble.word_id == aux->dato_lista_doble.word_id){
                        aux = aux->siguiente;break;
                    }else if(nuevo->dato_lista_doble.word_id < aux->dato_lista_doble.word_id){
                        nuevo->siguiente = lista->inicio;
                        lista->inicio = nuevo;
                        lista->tamanio_lista_doble++;break;
                    }else{
                        if(nuevo->dato_lista_doble.word_id > aux->dato_lista_doble.word_id && aux2 == NULL){
                            aux->siguiente = nuevo;
                            nuevo->siguiente = NULL;
                            lista->tamanio_lista_doble++;break;
                        }else{
                            if(aux->dato_lista_doble.word_id < nuevo->dato_lista_doble.word_id && aux->dato_lista_doble.word_id > nuevo->dato_lista_doble.word_id){
                                aux->siguiente = nuevo;
                                nuevo->siguiente = aux2;
                                lista->tamanio_lista_doble++;//return this;
                            }else{
                                aux = aux->siguiente;
                            }
                        }
                    }
                }
            }
        }else{
            cout << "ERROR EN PALABRA" << endl;
        }
    }else{
        cout << "NO SE INSERTO: "<< datos.word_id << endl;
    }
}


/*
 * Eliminar elemento de lista dobel */
void eliminar_de_lista_doble(LISTADOBLE *lista, DATOLISTADOBLE dato){
    NODOLISTADOBLE *temp = NULL;
    if(lista_doble_vacia(lista)){
        cout << "Lista doble vacía! xS" << endl;
    }else{
        if(lista->final == lista->inicio){printf("    INI == FIN\n");
            if(strcmp(lista->inicio->dato_lista_doble.word_lexema, dato.word_lexema) == 0){
                lista->tamanio_lista_doble = 0;
                lista->inicio = lista->final = NULL;
            }
        }else if(strcmp(lista->inicio->dato_lista_doble.word_lexema, dato.word_lexema) == 0){printf("    INI == dat\n");
            temp = lista->inicio->siguiente;
            lista->inicio->siguiente = NULL;
            temp->anterior = NULL;
            lista->inicio = temp; //temp = NULL;
            lista->tamanio_lista_doble--;
        }else if(strcmp(lista->final->dato_lista_doble.word_lexema, dato.word_lexema) == 0){printf("    FIN == dat\n");
            temp = lista->final->anterior;
            lista->final->anterior = NULL;
            temp->siguiente = NULL;
            lista->final = temp;//temp = NULL;
            lista->tamanio_lista_doble--;
        }else{
            temp = lista->inicio->siguiente;
            while(temp->siguiente != NULL){
                if(strcmp(temp->dato_lista_doble.word_lexema, dato.word_lexema) == 0){printf("    dat == dat\n");
                    NODOLISTADOBLE *actual = temp->anterior;
                    actual->siguiente = temp->siguiente;
                    temp->anterior = temp->anterior;//temp = NULL;
                    lista->tamanio_lista_doble--;
                }
                temp = temp->siguiente;
            }
        }
    }
}


/*
 * Eliminar elemento de lista dobel */
void eliminar_de_lista_doble_tmp(LISTADOBLE *lista, DATOLISTADOBLE dato){
    NODOLISTADOBLE *temp = NULL;
    if(lista_doble_vacia(lista)){
        cout << "Lista doble vacía! xS" << endl;
    }else{
        if(lista->final == lista->inicio){
            if(strcmp(lista->inicio->dato_lista_doble.word_lexema, dato.word_lexema) == 0){
                lista->tamanio_lista_doble = 0;
                lista->inicio = lista->final = NULL;
            }
        }/*else if(strcmp(lista->inicio->dato_lista_doble.word_lexema, dato.word_lexema) == 0){
            temp = lista->inicio->siguiente;
            lista->inicio->siguiente = NULL;
            temp->anterior = NULL;
            lista->inicio = temp; temp = NULL;
            lista->tamanio_lista_doble--;
        }else if(strcmp(lista->final->dato_lista_doble.word_lexema, dato.word_lexema) == 0){
            temp = lista->final->anterior;
            lista->final->anterior = NULL;
            temp->siguiente = NULL;
            lista->final = temp;temp = NULL;
            lista->tamanio_lista_doble--;
        }*/else{
            temp = lista->inicio->siguiente;
            while(temp->siguiente != NULL){
                if(strcmp(temp->dato_lista_doble.word_lexema, dato.word_lexema) == 0){
                    NODOLISTADOBLE *actual = temp->anterior;
                    actual->siguiente = temp->siguiente;
                    temp->anterior = temp->anterior;temp = NULL;
                    lista->tamanio_lista_doble--;
                }
                temp = temp->siguiente;
            }
        }
    }
}



/*
 * Mostrar elementos de lista doble */
void mostrar_lista_doble(LISTADOBLE *lista){
    NODOLISTADOBLE *aux = lista->inicio;
    if(!lista_doble_vacia(lista)){
        for(int i = 0; i < lista->tamanio_lista_doble ;i++){
            //printf("ID: %d, LEX: %s\n",raiz->dato_avl.word_id ,raiz->dato_avl.word_lexema);
            cout << "   ID: "<< aux->dato_lista_doble.word_id << ", LEX: "<< aux->dato_lista_doble.word_lexema << endl;
            aux = aux->siguiente;
        }
    }
}















