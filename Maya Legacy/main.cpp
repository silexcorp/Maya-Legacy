#include <QApplication>
#include "s_data.h"
#include "ventana.h"

/*
 * Variables globales */
ARBOLAVL s_data::arbol;
Ventana *s_data::ventana;
bool s_data::auto_save, s_data::auto_open;
char *s_data::setting_autor, *s_data::setting_correo;
char *s_data::lista_palabras[50000];

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*
     * Inicializar variables globales */
    s_data::ventana = new Ventana();
    s_data::ventana->show();
    s_data::arbol = NULL;
    s_data::auto_save = s_data::auto_open = 0 ;
    s_data::setting_autor = s_data::setting_correo = NULL;

    cargar_configuraciones();

    if(s_data::auto_open == 1){
        cargar_datos();
    }
    s_data::ventana->on_boton_actualizar_clicked();

    return a.exec();
}
