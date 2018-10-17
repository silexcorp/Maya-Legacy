#include "configuraciones.h"
#include "ui_configuraciones.h"
#include "s_data.h"

Configuraciones::Configuraciones(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Configuraciones)
{
    ui->setupUi(this);
    /* Qt::Window | Qt::SplashScreen | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint */
    this->setWindowFlags(Qt::SplashScreen);
    inicializar();
}

Configuraciones::~Configuraciones()
{
    delete ui;
}

/* Crear variable global en ventana */
static std::ofstream crear_salida;

void Configuraciones::inicializar(){
    ui->checkBox_open->setChecked(s_data::auto_open);
    ui->checkBox_save->setChecked(s_data::auto_save);
    QString autor = QString::asprintf(s_data::setting_autor);
    ui->lineEdit_nombre->setText(autor);
    QString correo = QString::asprintf(s_data::setting_correo);
    ui->lineEdit_correo->setText(correo);
}

/* Método para establecer el archivo
 * de salida y empezar a escribir datos */
void Configuraciones::escribri_config(){
    QFile f("config/config.ini");
    crear_salida.open(f.fileName().toUtf8());
    if(crear_salida.is_open()){
        //crear_salida << "autoopen, autosave" << endl;          //Headings for file
        for(int i = 0; i < 1; i++){
            crear_salida << "autoopen: " << s_data::auto_open << "\n"
                         << "autosave: " << s_data::auto_save << "\n"
                         << "autor: " << s_data::setting_autor << "\n"
                         << "correo: " << s_data::setting_correo << endl;
        }
    }
    crear_salida.close();
}

void Configuraciones::on_boton_aceptar_clicked()
{
    if(ui->checkBox_open->isChecked()){
        s_data::auto_open = 1;
        //qDebug()<<"checked";
    }else{
        s_data::auto_open = 0;
        //qDebug()<<"unchecked";
    }if(ui->checkBox_save->isChecked()){
        s_data::auto_save = 1;
        //qDebug()<<"checked";
    }else{
        s_data::auto_save = 0;
        //qDebug()<<"unchecked";
    }
    s_data::setting_autor ='\0';
    s_data::setting_autor = new char [strlen(ui->lineEdit_nombre->text().toStdString().c_str())+1];
    strcpy(s_data::setting_autor, ui->lineEdit_nombre->text().toStdString().c_str());
    s_data::setting_correo ='\0';
    s_data::setting_correo = new char [strlen(ui->lineEdit_correo->text().toStdString().c_str())+1];
    strcpy(s_data::setting_correo, ui->lineEdit_correo->text().toStdString().c_str());
    escribri_config();
    this->close();
    s_data::ventana->show();
}



void Configuraciones::on_boton_cancelar_clicked()
{
    this->close();
    s_data::ventana->show();
}
