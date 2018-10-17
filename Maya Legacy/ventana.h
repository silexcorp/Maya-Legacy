#ifndef VENTANA_H
#define VENTANA_H

#include <QMainWindow>
#include <QListWidget>
#include "s_arbol_avl.h"
#include <QPainter>

namespace Ui {
class Ventana;
}

class Ventana : public QMainWindow
{
    Q_OBJECT

public:
    explicit Ventana(QWidget *parent = 0);
    ~Ventana();

    void inicializar_tabla();
    void llenar_tabla();
    void llenar_lista();
    void re_generar_tabla();
    void obtener_datos(ARBOLAVL raiz);
    void geneara_salida(ARBOLAVL raiz);
    void escribri_csv();
    void nueva_entrada();
    void actualizar_lista();
    void obtener_informacion_para_pdf(ARBOLAVL raiz);

public slots:
    void on_boton_actualizar_clicked();

private slots:
    void acerca_de();
    void abrir_graficas();


    int exportar_pdf();


    void abrir_configuraciones();

    void on_boton_abrir_clicked();

    void on_boton_guardar_clicked();

    void on_boton_agregar_clicked();

    void on_boton_eliminar_clicked();

    void on_boton_buscar_clicked();

    void on_boton_cortar_clicked();

    void on_boton_copiar_clicked();

    void on_boton_pegar_clicked();

    void on_boton_anterior_clicked();

    void on_boton_siguiente_clicked();

    void on_boton_primero_clicked();

    void on_boton_ultimo_clicked();

    void on_boton_cerrar_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_boton_editar_clicked();

private:
    Ui::Ventana *ui;
};

#endif // VENTANA_H
