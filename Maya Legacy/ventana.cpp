#include "ventana.h"
#include "ui_ventana.h"
#include "configuraciones.h"
#include "s_data.h"
#include <iostream>
#include <fstream>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QString>

Ventana::Ventana(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ventana)
{
    ui->setupUi(this);
    /* Qt::Window | Qt::SplashScreen | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint */
    this->setWindowFlags(Qt::WindowTitleHint);
    inicializar_tabla();

    /*
     * Asignación de la conexión con los metodos mediante (SLOTS)
     * Las realciones de los botones con sus respectivas acciones */
    connect(ui->action_acerca_de, SIGNAL(triggered()), this, SLOT(acerca_de()));
    connect(ui->actionSalir, SIGNAL(triggered()), this, SLOT(on_boton_cerrar_clicked()));
    connect(ui->actionAbrir, SIGNAL(triggered()), this, SLOT(on_boton_abrir_clicked()));
    connect(ui->actionConfiguraciones, SIGNAL(triggered()), this, SLOT(abrir_configuraciones()));
    connect(ui->actionGraficas, SIGNAL(triggered()), this, SLOT(abrir_graficas()));

    connect(ui->actionPDF, SIGNAL(triggered()), this, SLOT(exportar_pdf()));

}

/* Crear variable global en ventana */
static std::ofstream crear_salida;
static int tam_datos, aux_tam_datos;
Ventana::~Ventana()
{
    delete ui;
}

/*
 *  Inicializar tabla */
void Ventana::inicializar_tabla(){
    //ui->tableWidget->resize(400, 250);
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    /* Crear la tabla e inicializar las filas y columnas */
    ui->tableWidget->setRowCount(22);
    ui->tableWidget->setColumnCount(1);

    /* Establever el tamaño de las celdas de la tabla */
    ui->tableWidget->setHorizontalHeaderLabels(QString("Descripcion                                                                                                                                              ").split(";"));
    ui->tableWidget->setVerticalHeaderLabels(QString("lexema;n sig;clase;subclase;sig esp;def;desc maya;desc esp;flexion;voc esp;ejemplo;trad ejem;sin;hom;dom lex;nota;ilus;ref maya;pres fuente;pres pal;fon;fecha").split(";"));

    /* Establecer el tamaño de los encabezados y no poder modificarlos */
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignLeft);
}

void Ventana::abrir_graficas(){
    graficar_avl(s_data::arbol);system ("grafo.png");
}

/*
 * Mostrar datos en widget lista
 * Mediante un recorride enorden */
void Ventana::obtener_datos(ARBOLAVL raiz){
    if(raiz != NULL){
        obtener_datos(raiz->izq);
            if(raiz->dato_avl.lista_colision != NULL){
                NODOLISTADOBLE *node = raiz->dato_avl.lista_colision->inicio;
                for(int i = 0; i < raiz->dato_avl.lista_colision->tamanio_lista_doble; i++){
                    s_data::lista_palabras[tam_datos] = node->dato_lista_doble.word_lexema;
                    node = node->siguiente;
                    tam_datos++;aux_tam_datos = tam_datos;
                }
            }
        obtener_datos(raiz->der);
    }
}

void Ventana::actualizar_lista(){
    char *temp;
    int i, j;
    for(i=0; i<tam_datos-1; i++){
       for(j=i+1; j<tam_datos; j++){
            if(strcmp(s_data::lista_palabras[i], s_data::lista_palabras[j]) > 0){
                temp = s_data::lista_palabras[i];
                s_data::lista_palabras[i] = s_data::lista_palabras[j];
                s_data::lista_palabras[j] = temp;
            }
        }
    }
    for(int i = 0; i < tam_datos; i++){
        QListWidgetItem *newItem;
        newItem = new QListWidgetItem;
        QString itemText = QString::fromStdString(s_data::lista_palabras[i]);
        newItem->setText(itemText);
        ui->listWidget->insertItem(i, newItem);
    }
}



/*
 * Llenar la lista widget mediante recorrido inoden ¨*/
void Ventana::llenar_lista(){
    tam_datos = aux_tam_datos = 0;
    ui->listWidget->clear();
    obtener_datos(s_data::arbol);
    actualizar_lista();
    /* Actualiza el total de palabras en base de datos*/
    QString cantid = QString::number(tam_datos);
    ui->label_palabras->setText(cantid);

}

/* Vacia la talba de datos */
void Ventana::llenar_tabla()
{
    ui->tableWidget->clearContents();
}

/* Re genera la talba inicializando la tabla otra vez */
void Ventana::re_generar_tabla(){
    ui->tableWidget->clearContents();
    inicializar_tabla();
}

/* Boton para ARBIR archivo y cargar datos */
void Ventana::on_boton_abrir_clicked()
{
    /* Primero guarda si esta trabajando uno */
    on_boton_guardar_clicked();
    /* Reestablecer datos */
    tam_datos = aux_tam_datos = 0;
    if(s_data::auto_open == 1){
        s_data::auto_open = 0;
        cargar_datos();
        s_data::auto_open = 1;
    }else{
        cargar_datos();
    }

    ui->tableWidget->clearContents();
    ui->listWidget->clear();
    llenar_tabla();
    llenar_lista();
}

/* Botono para GUARDAR */
void Ventana::on_boton_guardar_clicked()
{
    if(s_data::arbol != NULL){
        escribri_csv();
    }
}

/* Boton para AGREGAR nueva palabra */
void Ventana::on_boton_agregar_clicked()
{
    if(ui->tableWidget->item(0,0) != 0){
        DATOAVL dato;
        std::string aux = ui->tableWidget->item(0,0)->text().toStdString();
        /*
         * 1 Agregar 'LEXEMA' PALABRA */
        dato.word_lexema = new char [aux.size()+1];
        strcpy(dato.word_lexema, aux.c_str());
        /*
         * 0 Agregar 'ID' PALABRA' */
        dato.word_id = convertir(dato.word_lexema);
        /*
         * 2 Agregar 'N SIG' PALABRA */
        if(ui->tableWidget->item(1,0) != 0){        //Verifica si la casilla no esta vacía
            aux = ui->tableWidget->item(1,0)->text().toStdString();
            dato.word_n_sig = new char [aux.size()+1];
            strcpy(dato.word_n_sig, aux.c_str());
        }else{                                      //Si lo está, vacia contenido
            dato.word_n_sig = new char [1];
            strcpy(dato.word_n_sig, "");
        }                                           //Se repite el mismo proceso a todos los campos
        /*
        * 3 Agregar 'CLSE' PALABRA */if(ui->tableWidget->item(2,0) != 0){
        aux = ui->tableWidget->item(2,0)->text().toStdString();
        dato.word_clase = new char [aux.size()+1];
        strcpy(dato.word_clase, aux.c_str());}else{
            dato.word_clase = new char [1];
            strcpy(dato.word_clase, "");
        }
        /*
         * 4 Agregar 'SUBCLASE' PALABRA */if(ui->tableWidget->item(3,0) != 0){
        aux = ui->tableWidget->item(3,0)->text().toStdString();
        dato.word_subclase = new char [aux.size()+1];
        strcpy(dato.word_subclase, aux.c_str());}else{
            dato.word_subclase = new char [1];
            strcpy(dato.word_subclase, "");
        }
        /*
         * 5 Agregar 'SIG ESP' PALABRA */if(ui->tableWidget->item(4,0) != 0){
        aux = ui->tableWidget->item(4,0)->text().toStdString();
        dato.word_sig_esp = new char [aux.size()+1];
        strcpy(dato.word_sig_esp, aux.c_str());}else{
            dato.word_sig_esp = new char [1];
            strcpy(dato.word_sig_esp, "");
        }
        /*
         * 6 Agregar 'DEF' PALABRA */if(ui->tableWidget->item(5,0) != 0){
        aux = ui->tableWidget->item(5,0)->text().toStdString();
        dato.word_def = new char [aux.size()+1];
        strcpy(dato.word_def, aux.c_str());}else{
            dato.word_def = new char [1];
            strcpy(dato.word_def, "");
        }
        /*
         * 7 Agregar 'DESCRIPCION MAYA' PALABRA */if(ui->tableWidget->item(6,0) != 0){
        aux = ui->tableWidget->item(6,0)->text().toStdString();
        dato.word_desc_maya = new char [aux.size()+1];
        strcpy(dato.word_desc_maya, aux.c_str());}else{
            dato.word_desc_maya = new char [1];
            strcpy(dato.word_desc_maya, "");
        }
        /*
         * 8 Agregar 'DESCRIPCION ESPAÑOL' PALABRA */if(ui->tableWidget->item(7,0) != 0){
        aux = ui->tableWidget->item(7,0)->text().toStdString();
        dato.word_desc_esp = new char [aux.size()+1];
        strcpy(dato.word_desc_esp, aux.c_str());}else{
            dato.word_desc_esp = new char [1];
            strcpy(dato.word_desc_esp, "");
        }
        /*
         * 9 Agregar 'FLEXION' PALABRA */if(ui->tableWidget->item(8,0) != 0){
        aux = ui->tableWidget->item(8,0)->text().toStdString();
        dato.word_flexion = new char [aux.size()+1];
        strcpy(dato.word_flexion, aux.c_str());}else{
            dato.word_flexion = new char [1];
            strcpy(dato.word_flexion, "");
        }
        /*
         * 10 Agregar 'VOC ESP' PALABRA */if(ui->tableWidget->item(9,0) != 0){
        aux = ui->tableWidget->item(9,0)->text().toStdString();
        dato.word_voc_esp = new char [aux.size()+1];
        strcpy(dato.word_voc_esp, aux.c_str());}else{
            dato.word_voc_esp = new char [1];
            strcpy(dato.word_voc_esp, "");
        }
        /*
         * 11 Agregar 'EJEMOLO' PALABRA */if(ui->tableWidget->item(10,0) != 0){
        aux = ui->tableWidget->item(10,0)->text().toStdString();
        dato.word_ejemplo = new char [aux.size()+1];
        strcpy(dato.word_ejemplo, aux.c_str());}else{
            dato.word_ejemplo = new char [1];
            strcpy(dato.word_ejemplo, "");
        }
        /*
         * 12 Agregar 'TRADUC EJEMPLO' PALABRA */if(ui->tableWidget->item(11,0) != 0){
        aux = ui->tableWidget->item(11,0)->text().toStdString();
        dato.word_trad_ejem = new char [aux.size()+1];
        strcpy(dato.word_trad_ejem, aux.c_str());}else{
            dato.word_trad_ejem = new char [1];
            strcpy(dato.word_trad_ejem, "");
        }
        /*
         * 13 Agregar 'SIN' PALABRA */if(ui->tableWidget->item(12,0) != 0){
        aux = ui->tableWidget->item(12,0)->text().toStdString();
        dato.word_sin = new char [aux.size()+1];
        strcpy(dato.word_sin, aux.c_str());}else{
            dato.word_sin = new char [1];
            strcpy(dato.word_sin, "");
        }
        /*
         * 14 Agregar 'HOM' PALABRA */if(ui->tableWidget->item(13,0) != 0){
        aux = ui->tableWidget->item(13,0)->text().toStdString();
        dato.word_hom = new char [aux.size()+1];
        strcpy(dato.word_hom, aux.c_str());}else{
            dato.word_hom = new char [1];
            strcpy(dato.word_hom, "");
        }
        /*
         * 15 Agregar 'DOM LEX' PALABRA */if(ui->tableWidget->item(14,0) != 0){
        aux = ui->tableWidget->item(14,0)->text().toStdString();
        dato.word_dom_lex = new char [aux.size()+1];
        strcpy(dato.word_dom_lex, aux.c_str());}else{
            dato.word_dom_lex = new char [1];
            strcpy(dato.word_dom_lex, "");
        }
        /*
         * 16 Agregar 'NOTA' PALABRA */if(ui->tableWidget->item(15,0) != 0){
        aux = ui->tableWidget->item(15,0)->text().toStdString();
        dato.word_nota = new char [aux.size()+1];
        strcpy(dato.word_nota, aux.c_str());}else{
            dato.word_nota = new char [1];
            strcpy(dato.word_nota, "");
        }
        /*
         * 17 Agregar 'ILUSTRACION' PALABRA */if(ui->tableWidget->item(16,0) != 0){
        aux = ui->tableWidget->item(16,0)->text().toStdString();
        dato.word_ilust = new char [aux.size()+1];
        strcpy(dato.word_ilust, aux.c_str());}else{
            dato.word_ilust = new char [1];
            strcpy(dato.word_ilust, "");
        }
        /*
         * 18 Agregar 'REF MAYA' PALABRA */if(ui->tableWidget->item(17,0) != 0){
        aux = ui->tableWidget->item(17,0)->text().toStdString();
        dato.word_ref_maya = new char [aux.size()+1];
        strcpy(dato.word_ref_maya, aux.c_str());}else{
            dato.word_ref_maya = new char [1];
            strcpy(dato.word_ref_maya, "");
        }
        /*
         * 19 Agregar 'PRES FUENTE' PALABRA */if(ui->tableWidget->item(18,0) != 0){
        aux = ui->tableWidget->item(18,0)->text().toStdString();
        dato.word_pres_fuente = new char [aux.size()+1];
        strcpy(dato.word_pres_fuente, aux.c_str());}else{
            dato.word_pres_fuente = new char [1];
            strcpy(dato.word_pres_fuente, "");
        }
        /*
         * 20 Agregar 'PRES PALABRA' PALABRA */if(ui->tableWidget->item(19,0) != 0){
        aux = ui->tableWidget->item(19,0)->text().toStdString();
        dato.word_pres_pal = new char [aux.size()+1];
        strcpy(dato.word_pres_pal, aux.c_str());}else{
            dato.word_pres_pal = new char [1];
            strcpy(dato.word_pres_pal, "");
        }
        /*
         * 21 Agregar 'FON' PALABRA */if(ui->tableWidget->item(20,0) != 0){
        aux = ui->tableWidget->item(20,0)->text().toStdString();
        dato.word_fon = new char [aux.size()+1];
        strcpy(dato.word_fon, aux.c_str());}else{
            dato.word_fon = new char [1];
            strcpy(dato.word_fon, "");
        }
        /*
         * 22 Agregar 'FECHA' PALABRA */if(ui->tableWidget->item(21,0) != 0){
        aux = ui->tableWidget->item(21,0)->text().toStdString();
        dato.word_fecha = new char [aux.size()+1];
        strcpy(dato.word_fecha, aux.c_str());}else{
            dato.word_fecha = new char [1];
            strcpy(dato.word_fecha, "");
        }
        /* Como esta agregando un nuevo dato, actualiza
         * el tamaño, agrega y luego actualiza la lista */
        tam_datos++;aux_tam_datos = tam_datos;
        insertar_en_avl(&s_data::arbol,dato);
        ui->listWidget->clear();
        llenar_lista();

        QMessageBox n; n.information(this,"Notificacion","Palabra agregada");
    }else{
        QMessageBox n; n.information(this,"Advertencia","Escriba una palabra");
    }
}

/* Boton para ELIMINAR palabra */
void Ventana::on_boton_eliminar_clicked()
{
    if(ui->lineEdit_word->text() != NULL){
        DATOAVL datos;
        /*
         * Buscar PALABRA */
         std::string aux = ui->lineEdit_word->text().toStdString();
         datos.word_lexema = new char [aux.size()+1];
         strcpy(datos.word_lexema, aux.c_str());
        /* Si existe palabra, la elimina y actualiza los datos */
         NODOLISTADOBLE *buscado = NULL;
         buscado = buscar_nodo(&s_data::arbol,datos);//Buscar
         if(buscado != NULL){
            eliminar_en_avl(&s_data::arbol,datos);
            tam_datos--;aux_tam_datos = tam_datos;
            ui->lineEdit_word->setText("");
            on_boton_actualizar_clicked();
         }else{
             QMessageBox n; n.information(this,"Advertencia","Escriba una palabra");
         }
    }else{
        QMessageBox n; n.information(this,"Advertencia","Escriba una palabra");
    }
}

/* Al buscar, verifica primero si existe el dato */
void Ventana::nueva_entrada(){
    ui->tableWidget->clearContents();
    DATOAVL datos;
    QDate createDate;
    createDate = QDate::currentDate();
    /*
     * Buscar PALABRA y si la encuentra se actualiza la talba y agrega la fecha */
     std::string aux = ui->lineEdit_word->text().toStdString();
     datos.word_lexema = new char [aux.size()+1];
     strcpy(datos.word_lexema, aux.c_str());
     ui->tableWidget->setItem(0, 0, new QTableWidgetItem(tr(datos.word_lexema)));
     ui->tableWidget->setItem(21, 0, new QTableWidgetItem(tr(createDate.toString().toUtf8().data())));

}

/* Boton para BUSCAR palabra, si encuentra
 * la palabra automaticamente carga los datos */
void Ventana::on_boton_buscar_clicked()
{
    if(ui->lineEdit_word->text() != NULL){
        DATOAVL datos;
        /*
         * Buscar PALABRA */
         std::string aux = ui->lineEdit_word->text().toStdString();
         datos.word_lexema = new char [aux.size()+1];
         strcpy(datos.word_lexema, aux.c_str());
         NODOLISTADOBLE *buscado = NULL;
         buscado = buscar_nodo(&s_data::arbol,datos);//Buscar
         if(buscado != NULL){
             ui->tableWidget->clearContents();

             ui->tableWidget->setItem(0, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_lexema)));
             ui->tableWidget->setItem(1, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_n_sig)));
             ui->tableWidget->setItem(2, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_clase)));
             ui->tableWidget->setItem(3, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_subclase)));
             ui->tableWidget->setItem(4, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_sig_esp)));
             ui->tableWidget->setItem(5, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_def)));
             ui->tableWidget->setItem(6, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_desc_maya)));
             ui->tableWidget->setItem(7, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_desc_esp)));
             ui->tableWidget->setItem(8, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_flexion)));
             ui->tableWidget->setItem(9, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_voc_esp)));
             ui->tableWidget->setItem(10, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_ejemplo)));
             ui->tableWidget->setItem(11, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_trad_ejem)));
             ui->tableWidget->setItem(12, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_sin)));
             ui->tableWidget->setItem(13, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_hom)));
             ui->tableWidget->setItem(14, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_dom_lex)));
             ui->tableWidget->setItem(15, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_nota)));
             ui->tableWidget->setItem(16, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_ilust)));
             ui->tableWidget->setItem(17, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_ref_maya)));
             ui->tableWidget->setItem(18, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_pres_fuente)));
             ui->tableWidget->setItem(19, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_pres_pal)));
             ui->tableWidget->setItem(20, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_fon)));
             ui->tableWidget->setItem(21, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_fecha)));
         }else{
            QMessageBox msgBox;
                    msgBox.setText("Palabra no existente!");
                    msgBox.setInformativeText("Desea registrarla?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Discard);
                    msgBox.setDefaultButton(QMessageBox::Cancel);
                    int elegido = msgBox.exec();
                    switch (elegido) {
                      case QMessageBox::Yes:
                            nueva_entrada();
                            break;
                      case QMessageBox::Discard:
                            QMessageBox msgbox;
                            msgbox.information(this,"Informacion!","Puede continuar!");
                            break;
                     }
         }

    }else{
        QMessageBox n; n.information(this,"Advertencia","Escriba una palabra");
    }

}

/* Boton para CORTAR */
void Ventana::on_boton_cortar_clicked()
{

}

/* Boton para COPIAR */
void Ventana::on_boton_copiar_clicked()
{

}

/* Boton para PEGAR */
void Ventana::on_boton_pegar_clicked()
{
acerca_de();
}

/* Boton para regresar al ANTERIOR elemento */
void Ventana::on_boton_anterior_clicked()
{
    if(s_data::arbol != NULL){
        aux_tam_datos--;
        if(aux_tam_datos >= 0 && aux_tam_datos <= tam_datos-1){
            QListWidgetItem *item; item = new QListWidgetItem();
            item->setText(ui->listWidget->item(aux_tam_datos)->text());    ;
            on_listWidget_itemClicked(item);
        }else{
            aux_tam_datos = tam_datos-1;
            QListWidgetItem *item; item = new QListWidgetItem();
            item->setText(ui->listWidget->item(aux_tam_datos)->text());    ;
            on_listWidget_itemClicked(item);
        }

    }
}

/* Boton para avanzar al SIGUIENTE elemento */
void Ventana::on_boton_siguiente_clicked()
{
    if(s_data::arbol != NULL){
        aux_tam_datos++;
        if(aux_tam_datos >= 0 && aux_tam_datos <= tam_datos-1){
            QListWidgetItem *item; item = new QListWidgetItem();
            item->setText(ui->listWidget->item(aux_tam_datos)->text());    ;
            on_listWidget_itemClicked(item);

        }else{
            aux_tam_datos = 0;
            QListWidgetItem *item; item = new QListWidgetItem();
            item->setText(ui->listWidget->item(aux_tam_datos)->text());    ;
            on_listWidget_itemClicked(item);
        }

    }

    //
}

/* Boton para ir al PRIMER elemento */
void Ventana::on_boton_primero_clicked()
{
    if(s_data::arbol != NULL){
        QListWidgetItem *item; item = new QListWidgetItem();
        item->setText(ui->listWidget->item(0)->text());
        on_listWidget_itemClicked(item);
    }
    //
}

/* Boton para ir al ULTIMO elemento */
void Ventana::on_boton_ultimo_clicked()
{
    if(s_data::arbol != NULL){
        QListWidgetItem *item; item = new QListWidgetItem();
        item->setText(ui->listWidget->item(tam_datos-1)->text());    ;
        on_listWidget_itemClicked(item);
    }

    //
}

/* Boton ACTUALIZAR */
void Ventana::on_boton_actualizar_clicked()
{
    ui->lineEdit_word->setText("");
    ui->tableWidget->clearContents();
    ui->listWidget->clear();
    llenar_tabla();
    llenar_lista();
}

/* Boton CERRAR */
void Ventana::on_boton_cerrar_clicked()
{
    QMessageBox msg;
    if(s_data::arbol != NULL){
        msg.setText("Salir del sistema");
        msg.setInformativeText("Desea guardar los cambios?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Discard);
        msg.setDefaultButton(QMessageBox::Cancel);
        int elegido = msg.exec();
        switch (elegido) {
           case QMessageBox::Yes:
            escribri_csv();
            s_data::ventana->close();
            break;
           case QMessageBox::Discard:
                /*
                 * Salir sin guardar */
                s_data::ventana->close();
               break;
         }
    }else{
        QMessageBox msgBox;
            msgBox.setText("Salir de la aplicacion");
            msgBox.setInformativeText("Esta segur@ de salir?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Discard);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            int elegido = msgBox.exec();
            switch (elegido) {
               case QMessageBox::Yes:
                if(s_data::arbol != NULL){
                    escribri_csv();
                }
                s_data::ventana->close();
                break;
               case QMessageBox::Discard:
                    /*
                     * Solo un mensaje XD */
                    QMessageBox::information(this,"Informacion","!Puede continuar!");
                   break;
             }
    }


}

/* Al hacer clic sobre list widget, selecciona una imagen */
void Ventana::on_listWidget_itemClicked(QListWidgetItem *item)
{
    DATOAVL datos;
    /*
     * Buscar PALABRA */
     std::string aux = item->text().toStdString();
     datos.word_lexema = new char [aux.size()+1];
     strcpy(datos.word_lexema, aux.c_str());

     /* Muestra la palabra en la caja de texto */
     ui->lineEdit_word->setText(item->text());

     NODOLISTADOBLE *buscado = NULL;
     buscado = buscar_nodo(&s_data::arbol,datos);//Buscar
     if(buscado != NULL){
         if(buscado->dato_lista_doble.word_lexema != NULL){ui->tableWidget->setItem(0, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_lexema)));}
         if(buscado->dato_lista_doble.word_n_sig != NULL){ui->tableWidget->setItem(1, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_n_sig)));}else{ui->tableWidget->setItem(1, 0, new QTableWidgetItem(tr(" ")));}
         if(buscado->dato_lista_doble.word_clase != NULL){ui->tableWidget->setItem(2, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_clase)));}
         if(buscado->dato_lista_doble.word_subclase != NULL){ui->tableWidget->setItem(3, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_subclase)));}
         if(buscado->dato_lista_doble.word_sig_esp != NULL){ui->tableWidget->setItem(4, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_sig_esp)));}
         if(buscado->dato_lista_doble.word_def != NULL){ui->tableWidget->setItem(5, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_def)));}
         if(buscado->dato_lista_doble.word_desc_maya != NULL){ui->tableWidget->setItem(6, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_desc_maya)));}
         if(buscado->dato_lista_doble.word_desc_esp != NULL){ui->tableWidget->setItem(7, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_desc_esp)));}
         if(buscado->dato_lista_doble.word_flexion != NULL){ui->tableWidget->setItem(8, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_flexion)));}
         if(buscado->dato_lista_doble.word_voc_esp != NULL){ui->tableWidget->setItem(9, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_voc_esp)));}
         if(buscado->dato_lista_doble.word_ejemplo != NULL){ui->tableWidget->setItem(10, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_ejemplo)));}
         if(buscado->dato_lista_doble.word_trad_ejem != NULL){ui->tableWidget->setItem(11, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_trad_ejem)));}
         if(buscado->dato_lista_doble.word_sin != NULL){ui->tableWidget->setItem(12, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_sin)));}
         if(buscado->dato_lista_doble.word_hom != NULL){ui->tableWidget->setItem(13, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_hom)));}
         if(buscado->dato_lista_doble.word_dom_lex != NULL){ui->tableWidget->setItem(14, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_dom_lex)));}
         if(buscado->dato_lista_doble.word_nota != NULL){ui->tableWidget->setItem(15, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_nota)));}
         if(buscado->dato_lista_doble.word_ilust != NULL){ui->tableWidget->setItem(16, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_ilust)));}
         if(buscado->dato_lista_doble.word_ref_maya != NULL){ui->tableWidget->setItem(17, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_ref_maya)));}
         if(buscado->dato_lista_doble.word_pres_fuente != NULL){ui->tableWidget->setItem(18, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_pres_fuente)));}
         if(buscado->dato_lista_doble.word_pres_pal != NULL){ui->tableWidget->setItem(19, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_pres_pal)));}
         if(buscado->dato_lista_doble.word_fon != NULL){ui->tableWidget->setItem(20, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_fon)));}
         if(buscado->dato_lista_doble.word_fecha != NULL){ui->tableWidget->setItem(21, 0, new QTableWidgetItem(tr(buscado->dato_lista_doble.word_fecha)));}
     }else{
             printf("    La palabra no existe en la lista\n");
     }

}

/*
 * Recorrido del arbol inorden, y obtener datos.
   Ecribe los datos del arbol en el archivo csv */
void Ventana::geneara_salida(ARBOLAVL raiz){
    if(raiz != NULL){
        geneara_salida(raiz->izq);
            if(raiz->dato_avl.lista_colision != NULL){
                NODOLISTADOBLE *node = raiz->dato_avl.lista_colision->inicio;
                for(int i = 0; i < raiz->dato_avl.lista_colision->tamanio_lista_doble; i++){
                    crear_salida << node->dato_lista_doble.word_lexema << ","
                                 << node->dato_lista_doble.word_n_sig << ","
                                 << node->dato_lista_doble.word_clase << ","
                                 << node->dato_lista_doble.word_subclase << ","
                                 << node->dato_lista_doble.word_sig_esp << ","
                                 << node->dato_lista_doble.word_def << ","
                                 << node->dato_lista_doble.word_desc_maya << ","
                                 << node->dato_lista_doble.word_desc_esp << ","
                                 << node->dato_lista_doble.word_flexion << ","
                                 << node->dato_lista_doble.word_voc_esp << ","
                                 << node->dato_lista_doble.word_ejemplo << ","
                                 << node->dato_lista_doble.word_trad_ejem << ","
                                 << node->dato_lista_doble.word_sin << ","
                                 << node->dato_lista_doble.word_hom << ","
                                 << node->dato_lista_doble.word_dom_lex << ","
                                 << node->dato_lista_doble.word_nota << ","
                                 << node->dato_lista_doble.word_ilust << ","
                                 << node->dato_lista_doble.word_ref_maya << ","
                                 << node->dato_lista_doble.word_pres_fuente << ","
                                 << node->dato_lista_doble.word_pres_pal << ","
                                 << node->dato_lista_doble.word_fon << ","
                                 << node->dato_lista_doble.word_fecha << endl;
                    node = node->siguiente;
                }
            }
        geneara_salida(raiz->der);
    }
}

/* Método para establecer el archivo
 * de salida y empezar a escribir datos */
void Ventana::escribri_csv(){
    QFile f("database/database.csv");
    crear_salida.open(f.fileName().toUtf8());
    if(crear_salida.is_open()){
        crear_salida << "lexema,n sig,clase,subclase,sig esp,def,desc maya,desc esp,flexion,voc esp,ejemplo,trad ejem,sin,hom,dom lex,nota,ilus,ref maya,pres fuente,pres pal,fon,fecha" << endl;          //Headings for file
        geneara_salida(s_data::arbol);
    }
    //crear_salida.flush();
    crear_salida.close();
}

/* Boton para EDITAR el contenido de alguna palabra */
void Ventana::on_boton_editar_clicked()
{
    if(ui->lineEdit_word->text() != NULL){
        DATOAVL datos;
        /*
         * Buscar PALABRA */
         std::string aux = ui->lineEdit_word->text().toStdString();
         datos.word_lexema = new char [aux.size()+1];
         strcpy(datos.word_lexema, aux.c_str());

         NODOLISTADOBLE *buscado = NULL;
         buscado = buscar_nodo(&s_data::arbol,datos);//Buscar
         if(buscado != NULL){
             if(ui->tableWidget->item(0,0) != 0){
                 DATOAVL dato;

                 std::string aux = ui->tableWidget->item(0,0)->text().toStdString();
                 /*
                  * 1 Cargar 'LEXEMA' PALABRA */
                 dato.word_lexema = new char [aux.size()+1];
                 strcpy(dato.word_lexema, aux.c_str());
                 /*
                  * 0 Cargar 'ID' PALABRA' */
                 dato.word_id = convertir(dato.word_lexema);
                 buscado->dato_lista_doble.word_n_sig = '\0';               //Limpia contenido de palabra anterior
                 /*
                  * 2 Cargar 'N SIG' PALABRA */
                 if(ui->tableWidget->item(1,0) != 0){                       //Verifica si no esta vacía
                 aux = ui->tableWidget->item(1,0)->text().toStdString();
                 buscado->dato_lista_doble.word_n_sig = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_n_sig, aux.c_str());
                 }else{                                                     //De lo contrario vacía la cacia para evitar
                     buscado->dato_lista_doble.word_n_sig = new char [1];
                     strcpy(buscado->dato_lista_doble.word_n_sig, "");
                 }
                 buscado->dato_lista_doble.word_clase = '\0';
                 /*
                 * 3 Cargar 'CLSE' PALABRA */if(ui->tableWidget->item(2,0) != 0){
                 aux = ui->tableWidget->item(2,0)->text().toStdString();
                 buscado->dato_lista_doble.word_clase = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_clase, aux.c_str());}else{
                     buscado->dato_lista_doble.word_clase = new char [1];
                     strcpy(buscado->dato_lista_doble.word_clase, "");
                 }
                 buscado->dato_lista_doble.word_subclase = '\0';
                 /*
                  * 4 Cargar 'SUBCLASE' PALABRA */if(ui->tableWidget->item(3,0) != 0){
                 aux = ui->tableWidget->item(3,0)->text().toStdString();
                 buscado->dato_lista_doble.word_subclase = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_subclase, aux.c_str());}else{
                     buscado->dato_lista_doble.word_subclase = new char [1];
                     strcpy(buscado->dato_lista_doble.word_subclase, "");
                 }
                 buscado->dato_lista_doble.word_sig_esp = '\0';
                 /*
                  * 5 Cargar 'SIG ESP' PALABRA */if(ui->tableWidget->item(4,0) != 0){
                 aux = ui->tableWidget->item(4,0)->text().toStdString();
                 buscado->dato_lista_doble.word_sig_esp = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_sig_esp, aux.c_str());}else{
                     buscado->dato_lista_doble.word_sig_esp = new char [1];
                     strcpy(buscado->dato_lista_doble.word_sig_esp, "");
                 }
                 buscado->dato_lista_doble.word_def = '\0';
                 /*
                  * 6 Cargar 'DEF' PALABRA */if(ui->tableWidget->item(5,0) != 0){
                 aux = ui->tableWidget->item(5,0)->text().toStdString();
                 buscado->dato_lista_doble.word_def = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_def, aux.c_str());}else{
                     buscado->dato_lista_doble.word_def = new char [1];
                     strcpy(buscado->dato_lista_doble.word_def, "");
                 }
                 buscado->dato_lista_doble.word_desc_maya = '\0';
                 /*
                  * 7 Cargar 'DESCRIPCION MAYA' PALABRA */if(ui->tableWidget->item(6,0) != 0){
                 aux = ui->tableWidget->item(6,0)->text().toStdString();
                 buscado->dato_lista_doble.word_desc_maya = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_desc_maya, aux.c_str());}else{
                     buscado->dato_lista_doble.word_desc_maya = new char [1];
                     strcpy(buscado->dato_lista_doble.word_desc_maya, "");
                 }
                 buscado->dato_lista_doble.word_desc_esp = '\0';
                 /*
                  * 8 Cargar 'DESCRIPCION ESPAÑOL' PALABRA */if(ui->tableWidget->item(7,0) != 0){
                 aux = ui->tableWidget->item(7,0)->text().toStdString();
                 buscado->dato_lista_doble.word_desc_esp = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_desc_esp, aux.c_str());}else{
                     buscado->dato_lista_doble.word_desc_esp = new char [1];
                     strcpy(buscado->dato_lista_doble.word_desc_esp, "");
                 }
                 buscado->dato_lista_doble.word_flexion = '\0';
                 /*
                  * 9 Cargar 'FLEXION' PALABRA */if(ui->tableWidget->item(8,0) != 0){
                 aux = ui->tableWidget->item(8,0)->text().toStdString();
                 buscado->dato_lista_doble.word_flexion = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_flexion, aux.c_str());}else{
                     buscado->dato_lista_doble.word_flexion = new char [1];
                     strcpy(buscado->dato_lista_doble.word_flexion, "");
                 }
                 buscado->dato_lista_doble.word_voc_esp = '\0';
                 /*
                  * 10 Cargar 'VOC ESP' PALABRA */if(ui->tableWidget->item(9,0) != 0){
                 aux = ui->tableWidget->item(9,0)->text().toStdString();
                 buscado->dato_lista_doble.word_voc_esp = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_voc_esp, aux.c_str());}else{
                     buscado->dato_lista_doble.word_voc_esp = new char [1];
                     strcpy(buscado->dato_lista_doble.word_voc_esp, "");
                 }
                 buscado->dato_lista_doble.word_ejemplo = '\0';
                 /*
                  * 11 Cargar 'EJEMOLO' PALABRA */if(ui->tableWidget->item(10,0) != 0){
                 aux = ui->tableWidget->item(10,0)->text().toStdString();
                 buscado->dato_lista_doble.word_ejemplo = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_ejemplo, aux.c_str());}else{
                     buscado->dato_lista_doble.word_ejemplo = new char [1];
                     strcpy(buscado->dato_lista_doble.word_ejemplo, "");
                 }
                 buscado->dato_lista_doble.word_trad_ejem = '\0';
                 /*
                  * 12 Cargar 'TRADUC EJEMPLO' PALABRA */if(ui->tableWidget->item(11,0) != 0){
                 aux = ui->tableWidget->item(11,0)->text().toStdString();
                 buscado->dato_lista_doble.word_trad_ejem = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_trad_ejem, aux.c_str());}else{
                     buscado->dato_lista_doble.word_trad_ejem = new char [1];
                     strcpy(buscado->dato_lista_doble.word_trad_ejem, "");
                 }
                 buscado->dato_lista_doble.word_sin = '\0';
                 /*
                  * 13 Cargar 'SIN' PALABRA */if(ui->tableWidget->item(12,0) != 0){
                 aux = ui->tableWidget->item(12,0)->text().toStdString();
                 buscado->dato_lista_doble.word_sin = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_sin, aux.c_str());}else{
                     buscado->dato_lista_doble.word_sin = new char [1];
                     strcpy(buscado->dato_lista_doble.word_sin, "");
                 }
                 buscado->dato_lista_doble.word_hom = '\0';
                 /*
                  * 14 Cargar 'HOM' PALABRA */if(ui->tableWidget->item(13,0) != 0){
                 aux = ui->tableWidget->item(13,0)->text().toStdString();
                 buscado->dato_lista_doble.word_hom = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_hom, aux.c_str());}else{
                     buscado->dato_lista_doble.word_hom = new char [1];
                     strcpy(buscado->dato_lista_doble.word_hom, "");
                 }
                 buscado->dato_lista_doble.word_dom_lex = '\0';
                 /*
                  * 15 Cargar 'DOM LEX' PALABRA */if(ui->tableWidget->item(14,0) != 0){
                 aux = ui->tableWidget->item(14,0)->text().toStdString();
                 buscado->dato_lista_doble.word_dom_lex = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_dom_lex, aux.c_str());}else{
                     buscado->dato_lista_doble.word_dom_lex = new char [1];
                     strcpy(buscado->dato_lista_doble.word_dom_lex, "");
                 }
                 buscado->dato_lista_doble.word_nota = '\0';
                 /*
                  * 16 Cargar 'NOTA' PALABRA */if(ui->tableWidget->item(15,0) != 0){
                 aux = ui->tableWidget->item(15,0)->text().toStdString();
                 buscado->dato_lista_doble.word_nota = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_nota, aux.c_str());}else{
                     buscado->dato_lista_doble.word_nota = new char [1];
                     strcpy(buscado->dato_lista_doble.word_nota, "");
                 }
                 buscado->dato_lista_doble.word_ilust = '\0';
                 /*
                  * 17 Cargar 'ILUSTRACION' PALABRA */if(ui->tableWidget->item(16,0) != 0){
                 aux = ui->tableWidget->item(16,0)->text().toStdString();
                 buscado->dato_lista_doble.word_ilust = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_ilust, aux.c_str());}else{
                     buscado->dato_lista_doble.word_ilust = new char [1];
                     strcpy(buscado->dato_lista_doble.word_ilust, "");
                 }
                 buscado->dato_lista_doble.word_ref_maya = '\0';
                 /*
                  * 18 Cargar 'REF MAYA' PALABRA */if(ui->tableWidget->item(17,0) != 0){
                 aux = ui->tableWidget->item(17,0)->text().toStdString();
                 buscado->dato_lista_doble.word_ref_maya = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_ref_maya, aux.c_str());}else{
                     buscado->dato_lista_doble.word_ref_maya = new char [1];
                     strcpy(buscado->dato_lista_doble.word_ref_maya, "");
                 }
                 buscado->dato_lista_doble.word_pres_fuente = '\0';
                 /*
                  * 19 Cargar 'PRES FUENTE' PALABRA */if(ui->tableWidget->item(18,0) != 0){
                 aux = ui->tableWidget->item(18,0)->text().toStdString();
                 buscado->dato_lista_doble.word_pres_fuente = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_pres_fuente, aux.c_str());}else{
                     buscado->dato_lista_doble.word_pres_fuente = new char [1];
                     strcpy(buscado->dato_lista_doble.word_pres_fuente, "");
                 }
                 buscado->dato_lista_doble.word_pres_pal = '\0';
                 /*
                  * 20 Cargar 'PRES PALABRA' PALABRA */if(ui->tableWidget->item(19,0) != 0){
                 aux = ui->tableWidget->item(19,0)->text().toStdString();
                 buscado->dato_lista_doble.word_pres_pal = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_pres_pal, aux.c_str());}else{
                     buscado->dato_lista_doble.word_pres_pal = new char [1];
                     strcpy(buscado->dato_lista_doble.word_pres_pal, "");
                 }
                 buscado->dato_lista_doble.word_fon = '\0';
                 /*
                  * 21 Cargar 'FON' PALABRA */if(ui->tableWidget->item(20,0) != 0){
                 aux = ui->tableWidget->item(20,0)->text().toStdString();
                 buscado->dato_lista_doble.word_fon = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_fon, aux.c_str());}else{
                     buscado->dato_lista_doble.word_fon = new char [1];
                     strcpy(buscado->dato_lista_doble.word_fon, "");
                 }
                 buscado->dato_lista_doble.word_fecha = '\0';
                 /*
                  * 22 Cargar 'FECHA' PALABRA */if(ui->tableWidget->item(21,0) != 0){
                 aux = ui->tableWidget->item(21,0)->text().toStdString();
                 buscado->dato_lista_doble.word_fecha = new char [aux.size()+1];
                 strcpy(buscado->dato_lista_doble.word_fecha, aux.c_str());}else{
                     buscado->dato_lista_doble.word_fecha = new char [1];
                     strcpy(buscado->dato_lista_doble.word_fecha, "");
                 }

                 //insertar_en_avl(&s_data::arbol,dato);
                 ui->listWidget->clear();
                 llenar_lista();
                 QMessageBox n; n.information(this,"Notificacion","Palabra editada");
             }else{
                 QMessageBox n; n.information(this,"Advertencia","Escriba una palabra");
             }

         }else{
             QMessageBox n; n.information(this,"Informacion","Palabra no encontrada");
         }
    }else{
        QMessageBox n; n.information(this,"Advertencia","Escriba una palabra");
    }

}


void Ventana::acerca_de(){
    QMessageBox::about(this, tr("Acerca de la aplicacion"),
                 tr("<b>Maya Legacy</b> aplicación escrito en lenguaje C++ con la finalidad de crear, editar y administrar la base de datos de un diccionario español/maya q'anjob'al"
                    ". Esta protegida bajo la licencia propietaria CORP, autor: <b>Alexander</b><b>Mateo</b> a cargo de la empresa <b>SilexCorp</b> liceincia unica al propietario actual <b>Felipe</b> "
                    " Para mayor información visite nuestro sitio web: <b>  http://silexCorp.org</b>  o <b> http://leerparaaprender.com </b> SILEXCORP 2016 "));

}

void Ventana::abrir_configuraciones(){
    Configuraciones *c; c = new Configuraciones();
    c->show();
    s_data::ventana->hide();
}

/*
 * Obtener datos del arbol para luego
 * exportarlos al archivo pdf diccionario  */
void Ventana::obtener_informacion_para_pdf(ARBOLAVL raiz){
    if(raiz != NULL){
        obtener_informacion_para_pdf(raiz->izq);
            if(raiz->dato_avl.lista_colision != NULL){
                NODOLISTADOBLE *node = raiz->dato_avl.lista_colision->inicio;
                for(int i = 0; i < raiz->dato_avl.lista_colision->tamanio_lista_doble; i++){
                    //QString texto = QString::asprintf(s_data::lista_palabras[i]);
                    //TODO
                    //painter.drawText(70,i*20 + 70,texto);
                    /*s_data::lista_palabras[tam_datos] = node->dato_lista_doble.word_lexema;
                    tam_datos++;aux_tam_datos = tam_datos;*/
                    node = node->siguiente;
                }
            }
        obtener_informacion_para_pdf(raiz->der);
    }
}

int Ventana::exportar_pdf(){
    QPrinter printer;
       printer.setOutputFormat(QPrinter::PdfFormat);
       //printer.setPageSize(QPageSize::Letter);
       printer.setOutputFileName("export/diccionario.pdf");
       QPainter painter;
       if (! painter.begin(&printer)) { // failed to open file
           qWarning("failed to open file, is it writable?");
           return 1;
       }
       painter.drawText(10, 20, "Maya Legacy - Diccionario Español - Maya Q'anjob'al - Página 1");
       for(int i = 0; i< tam_datos; i ++){
           //QString texto = QString::asprintf(s_data::lista_palabras[i]);
           //painter.drawText(70,i*20 + 70,texto);
           //TODO
       }

       //obtener_informacion_para_pdf(s_data::arbol);


       if (! printer.newPage()) {
           qWarning("failed in flushing page to disk, disk full?");
           return 1;
       }
       painter.drawText(10, 20, "Maya Legacy - Diccionario Español - Maya Q'anjob'al - página 2");
       painter.end();
       QMessageBox m; m.information(this,"Informacion","Archivo exportado correctamente");
       //system("export/dic.pdf");
}
