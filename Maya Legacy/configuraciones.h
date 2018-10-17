#ifndef CONFIGURACIONES_H
#define CONFIGURACIONES_H

#include <QMainWindow>
#include <QCheckBox>
#include <QMessageBox>

namespace Ui {
class Configuraciones;
}

class Configuraciones : public QMainWindow
{
    Q_OBJECT

public:
    explicit Configuraciones(QWidget *parent = 0);
    ~Configuraciones();
    void escribri_config();
    void inicializar();

public slots:

private slots:
    void on_boton_aceptar_clicked();

    void on_boton_cancelar_clicked();


private:
    Ui::Configuraciones *ui;
};

#endif // CONFIGURACIONES_H
