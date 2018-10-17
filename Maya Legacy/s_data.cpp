#include "s_data.h"


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

s_data::s_data()
{
}

int convertir(char *dato){
    int longitud = 0;
        for (int x = 0; x < strlen(dato); x++){
            longitud += (int)dato[x];
        }
        for (int x = 0; x < strlen(dato)/2; x++){
            longitud += (int)dato[x];
            //printf("\n %c codificado es: %d", dato[x], (int)dato[x]);
        }
        //printf("%s codificado es: %d\n", dato, longitud);
    return longitud;
}

void cargar_configuraciones(){
    FILE * archivo;int c,d =0;
    char cadena [200];
    archivo = fopen ("config/config.ini","r");
    if (archivo == NULL) perror ("Error al abrir el archivo");
    else
    {
        while (feof(archivo) == 0)
        {
            fgets (cadena , 200 , archivo);
            char *nueva_cadena;
            char *temporal;
            //printf ("Contiene: %s\n",cadena);
            nueva_cadena = strtok (cadena,":");
            while (nueva_cadena != NULL)
            {
                //printf ("Con split: %s\n",nueva_cadena);
                if(c != 0){
                    temporal = strtok(nueva_cadena, " ");
                    if(temporal){
                        /*
                         * Cargar 'CONFIGURACIONES' del sistema */
                        if(d  == 0){
                            /*const char *charVar = "16";
                            unsigned int uintVar = 0;
                            uintVar = atoi(charVar);*/

                            s_data::auto_open = atoi(temporal);
                            //printf("1) OPEN: %d\n", s_data::auto_open);
                            d++;
                        }else if(d == 1){
                            s_data::auto_save = atoi(temporal);
                            //printf("2) SAVE: %d\n", s_data::auto_save);
                            d++;
                        }else if(d == 2){
                            s_data::setting_autor = new char [strlen(temporal)+1];
                            strcpy(s_data::setting_autor, temporal);
                            //printf("3) AUTOR: %s\n", s_data::setting_autor);
                            d++;
                        }else if(d == 3){
                            s_data::setting_correo = new char [strlen(temporal)+1];
                            strcpy(s_data::setting_correo, temporal);
                            //printf("4) CORREO: %s\n", s_data::setting_correo);
                            d++;
                        }

                    }
                    temporal = strtok(NULL, "");
                }
                c++;
                nueva_cadena = strtok (NULL, ":");
            }
            c=0;
        }
        //printf("\nCarga exitosa! Juegos de suarios agregados exitosamente. \n");
        fclose (archivo);
    }
}

/*
 * Retorna un vector resultante de la lectura del archivo */
std::vector <std::string>  choose_file(){
    std::string direccion;const char *cadena;

    if(s_data::auto_open == 1){
        QFile f("database/database.csv");
        direccion = f.fileName().toStdString();
        cadena = direccion.c_str();
    }else{
        QString dir = QFileDialog::getOpenFileName(NULL,QObject::tr("Abrir archivo csv"), "", QObject::tr("csv (*.csv)"));
        direccion = dir.toUtf8().constData();
        cadena = direccion.c_str();
    }
    std::ifstream datos(cadena);
    std::string line;
    std::vector <std::string> info;
    try
    {
        while(getline(datos,line))
        {
        std::stringstream lineStream(line);
        std::string cell;
            while(std::getline(lineStream,cell,','))
            {
                info.push_back(cell);
            }
        }
        return info;
    }
    catch(exception &e){
    /* Ha ocurrido un error x) */
        puts("¡Error al cargar archivo!");
        QMessageBox msgbox;
        msgbox.information(NULL,"!Error al cargar archivo!",e.what());
    }
    return info;/* Adicional, es por el retorno, quitar si genera problemas :D */
}


void cargar_datos(){

    std::vector <std::string> dato_palabra = choose_file();
    if(!dato_palabra.empty()){
        int tam = dato_palabra.size();

        for(int i = 22; i < tam; i++){
            DATOAVL dato;
            std::string aux = dato_palabra.at(i);

            /*
             * 1 Cargar 'LEXEMA' PALABRA */
            dato.word_lexema = new char [aux.size()+1];
            strcpy(dato.word_lexema, aux.c_str());
            /*
             * 0 Cargar 'ID' PALABRA' */
            dato.word_id = convertir(dato.word_lexema);
            i++;

            /*
             * 2 Cargar 'N SIG' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_n_sig = new char [aux.size()+1];
            strcpy(dato.word_n_sig, aux.c_str());
            i++;
            /*
             * 3 Cargar 'CLSE' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_clase = new char [aux.size()+1];
            strcpy(dato.word_clase, aux.c_str());
            i++;
            /*
             * 4 Cargar 'SUBCLASE' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_subclase = new char [aux.size()+1];
            strcpy(dato.word_subclase, aux.c_str());
            i++;
            /*
             * 5 Cargar 'SIG ESP' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_sig_esp = new char [aux.size()+1];
            strcpy(dato.word_sig_esp, aux.c_str());
            i++;
            /*
             * 6 Cargar 'DEF' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_def = new char [aux.size()+1];
            strcpy(dato.word_def, aux.c_str());
            i++;
            /*
             * 7 Cargar 'DESCRIPCION MAYA' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_desc_maya = new char [aux.size()+1];
            strcpy(dato.word_desc_maya, aux.c_str());
            i++;
            /*
             * 8 Cargar 'DESCRIPCION ESPAÑOL' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_desc_esp = new char [aux.size()+1];
            strcpy(dato.word_desc_esp, aux.c_str());
            i++;
            /*
             * 9 Cargar 'FLEXION' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_flexion = new char [aux.size()+1];
            strcpy(dato.word_flexion, aux.c_str());
            i++;
            /*
             * 10 Cargar 'VOC ESP' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_voc_esp = new char [aux.size()+1];
            strcpy(dato.word_voc_esp, aux.c_str());
            i++;
            /*
             * 11 Cargar 'EJEMOLO' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_ejemplo = new char [aux.size()+1];
            strcpy(dato.word_ejemplo, aux.c_str());
            i++;
            /*
             * 12 Cargar 'TRADUC EJEMPLO' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_trad_ejem = new char [aux.size()+1];
            strcpy(dato.word_trad_ejem, aux.c_str());
            i++;
            /*
             * 13 Cargar 'SIN' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_sin = new char [aux.size()+1];
            strcpy(dato.word_sin, aux.c_str());
            i++;
            /*
             * 14 Cargar 'HOM' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_hom = new char [aux.size()+1];
            strcpy(dato.word_hom, aux.c_str());
            i++;
            /*
             * 15 Cargar 'DOM LEX' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_dom_lex = new char [aux.size()+1];
            strcpy(dato.word_dom_lex, aux.c_str());
            i++;
            /*
             * 16 Cargar 'NOTA' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_nota = new char [aux.size()+1];
            strcpy(dato.word_nota, aux.c_str());
            i++;
            /*
             * 17 Cargar 'ILUSTRACION' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_ilust = new char [aux.size()+1];
            strcpy(dato.word_ilust, aux.c_str());
            i++;
            /*
             * 18 Cargar 'REF MAYA' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_ref_maya = new char [aux.size()+1];
            strcpy(dato.word_ref_maya, aux.c_str());
            i++;
            /*
             * 19 Cargar 'PRES FUENTE' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_pres_fuente = new char [aux.size()+1];
            strcpy(dato.word_pres_fuente, aux.c_str());
            i++;
            /*
             * 20 Cargar 'PRES PALABRA' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_pres_pal = new char [aux.size()+1];
            strcpy(dato.word_pres_pal, aux.c_str());
            i++;
            /*
             * 21 Cargar 'FON' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_fon = new char [aux.size()+1];
            strcpy(dato.word_fon, aux.c_str());
            i++;
            /*
             * 22 Cargar 'FECHA' PALABRA */
            aux = dato_palabra.at(i);
            dato.word_fecha = new char [aux.size()+1];
            strcpy(dato.word_fecha, aux.c_str());
            /*
             * Verificar si la palabra existe en el nodo de colisiones y si
             * el elemento en el árbol asociado existe, se ingresa en la
             * lista de colisiones del nodo del árbol, si se repite no se ingresa */

            insertar_en_avl(&s_data::arbol,dato);

        }
        puts("");


        //QMessageBox msgbox;
        //msgbox.information(s_data::ventana,"Carga exitosa!","Palabras cargadas exitosamente.");

        //graficar_avl(s_data::arbol);

    }else{
        QMessageBox msgbox;
        //msgbox.information(s_data::ventana,"¡Carga fallida!","Error, ¡archivo vacio!");
    }

}








/**
 * @brief generar_reporte
 * @param lista
 * @param nombre
 */
void generar_reporte_lista_doble(LISTADOBLE *lista, char *nombre){
    FILE* grafo = fopen("grafo.dot","w+");
    fprintf(grafo,"digraph grafo{\n");
    fprintf(grafo,"\trank=same;\n");
    fprintf(grafo,"\tnodesep=0.5;\n");//[shape=record,color=lightblue];
    fprintf(grafo,"\tlabel=\"LISTA CIRCULAR %s\";\n",nombre);
    fprintf(grafo,"\tbgcolor=\"white\";\n");
    /*LR=Izquierda a derecha || TB=Arriba a abajo*/
   // fprintf(grafo,"\trankdir=LR\n");
    /*
     * Definir atributos generales de los nodos */
    fprintf(grafo,"\tnode [shape=record];\n");
    /*
     * Definicion atributos generales aristas */
    fprintf(grafo,"\tedge [color=\"lightblue\", dir=fordware];\n");

    /**
     * @brief graficar_lista_simple
     * Mandar a llamar metodo graficar y definir nodos*/
    graficar_lista_doble(grafo,lista, nombre);

    fprintf(grafo,"}");
    fclose(grafo);
    system("dot -Tpng grafo.dot -o grafo.png ");
    system ("grafo.png");

}


void graficar_lista_doble(FILE *grafo, LISTADOBLE *lista, char *name){
    NODOLISTADOBLE *aux_doble;
    aux_doble = lista->inicio;

    //fprintf(grafo,"\t\tcabecera_%s [label=\"cabecera_%s\", shape = none];\n",name,name);
    for(int x = 0; x < lista->tamanio_lista_doble; x++){
        if(aux_doble != NULL){
            int i = 0;
            fprintf(grafo,"\n\tsubgraph cluster_listaDoble_%s {\n",name);

            /*
             * Definir atributos generales de los nodos */
            fprintf(grafo,"\tnode [color=skyblue, style=filled];\n");
            /*LR=Izquierda a derecha || TB=Arriba a abajo*/
            fprintf(grafo,"\trankdir=TB\n");
            /*  Definir relaciones entre nodos */
            fprintf(grafo,"\n");
            fprintf(grafo,"\n\t\tlabel = \"LISTA DE %s\";\n", name);
            fprintf(grafo,"\t\tcabecera_%s [label=\"CABECERA %s\", shape = none];\n",name,name);
            while(aux_doble != NULL){
                fprintf(grafo,"\t\tlista_%s%d [label=\"<a> | <b> ID: %d \\n NAME: %s | <c>\"];\n",name,i,aux_doble->dato_lista_doble.word_id,aux_doble->dato_lista_doble.word_lexema);
                aux_doble = aux_doble->siguiente; i++;
            }

            fprintf(grafo,"\t\tnull_%s [label = \"null_%s\", shape = none];\n", name, name);
            i = 0;
            fprintf(grafo,"\n");
            fprintf(grafo,"\t}\n");

            fprintf(grafo,"\tcabecera_%s -> lista_%s0;\n",name, name);

            aux_doble = lista->inicio;
            while(aux_doble != NULL){
                if(aux_doble->siguiente != NULL){
                    fprintf(grafo,"\tlista_%s%d -> lista_%s%d;\n",name,i,name,i+1);
                }
                aux_doble = aux_doble->siguiente; i++;
            }
            aux_doble = lista->inicio;
            i--;i--;
            /* INVERTIR FLECHAS XD */
            while(aux_doble != NULL){
                if(aux_doble->siguiente != NULL){
                    fprintf(grafo,"\tlista_%s%d -> lista_%s%d;\n",name,i+1,name,i);
                }
                aux_doble = aux_doble->siguiente; i--;
            }
            fprintf(grafo,"\tlista_%s%d -> null_%s;\n",name,lista->tamanio_lista_doble-1,name);

        }else{

        }

    }

}





void escribir_nodo(FILE *grafo,NODOAVL *nodo){
    if (nodo == NULL) return;

    fprintf(grafo,"n_%d [label = \"<der> | <dat> ID: %d \\n LEX: %s |<izq>\"];\n",nodo->dato_avl.word_id,nodo->dato_avl.word_id,nodo->dato_avl.word_lexema);
    NODOAVL *izq = nodo->izq;
    NODOAVL *der = nodo->der;

    if (der != NULL)
        fprintf(grafo,"n_%d:der -> n_%d:dat;\n",nodo->dato_avl.word_id,der->dato_avl.word_id);
    if (izq != NULL)
        fprintf(grafo,"n_%d:izq -> n_%d:dat;\n",nodo->dato_avl.word_id,izq->dato_avl.word_id);

    fflush(grafo);
    escribir_nodo(grafo,izq);
    escribir_nodo(grafo,der);
}

void graficar_avl(NODOAVL *raiz){
    FILE *grafo = fopen("grafo.dot","w+");
    fprintf(grafo,"digraph grafo{\n");
    fprintf(grafo,"\trank=same;\n");
    fprintf(grafo,"\tnodesep=0.5;\n");//[shape=record,color=lightblue];
    fprintf(grafo,"\tlabel=\"ARBOL DE %s\"\n","PALABRAS");
    fprintf(grafo,"\tbgcolor=\"white\"\n");//
     // Definir atributos generales de los nodos
    fprintf(grafo,"\tnode [shape = record,color=skyblue, style=filled];\n");
     // Definicion atributos generales aristas /
    fprintf(grafo,"\tedge [color=\"skyblue\", dir=fordware];\n");
    fprintf(grafo,"rankdir = TD;\n");

    escribir_nodo(grafo,raiz);

    fprintf(grafo,"}");
    fclose(grafo);
    system("dot -Tpng grafo.dot -o grafo.png ");

}

