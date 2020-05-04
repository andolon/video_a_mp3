#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDir>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QList>
#include <QDateTime>
#include <QMessageBox>

#include "definiciones.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void revisa_requerimientos(void);

private:
    Ui::MainWindow *ui;

    QString ruta_inicio;

    //QProcess _proceso;

    QString dialogo_directorio(QString inicio, QWidget *parent);
    void procesar(Tipo_Archivo_musica archivo, int tipo_archivo);
    void lote_proceso(void);
    QStringList lista_audio;
    QStringList lista_video;
    QList<Tipo_Archivo_musica> archivos_video;
    QList<Tipo_Archivo_musica> archivos_audio;
    QDateTime fecha;

    int progreso;

    volatile bool abortar;


private slots:

        void on_pushButton_selecciona_ruta_clicked();
        void corre_find(void);
        void cambia_rutas(void);
        void on_pushButton_inicia_clicked();
        void on_pushButton_abortar_clicked();


};

#endif // MAINWINDOW_H
