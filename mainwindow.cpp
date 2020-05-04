#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->setStyleSheet(myStilo);

    fecha = QDateTime::currentDateTime();

    ui->label_trabajando->setText("");
    ui->label_origen->setText("");
    ui->label_destino->setText("");

    progreso = 0;

    abortar = false;

    //QString myUnixPath = QDir::toNativeSeparators("/home/path with spaces/");

    //qDebug() << myUnixPath;

    //http://www.bogotobogo.com/Qt/Qt5_QThreads_GuiThread.php

}

void MainWindow::revisa_requerimientos(void)
{
    if(false == QFileInfo::exists(FFMPEG))
    {
        QMessageBox msgBox;
        msgBox.move(50,50);
        msgBox.setText(tr("Alerta"));
        msgBox.setInformativeText(QString("No existe herramienta %1").arg(FFMPEG));
        msgBox.exec();

        exit (0);

        return;
    }

    /*
    if(false == QFileInfo::exists(BUSCA))
    {
        QMessageBox msgBox;
        msgBox.move(50,50);
        msgBox.setText(tr("Alerta"));
        msgBox.setInformativeText(QString("No existe herramienta %1").arg(BUSCA));
        msgBox.exec();

        exit (0);

        return;
    }*/

}

QString MainWindow::dialogo_directorio(QString inicio, QWidget *parent)
{
    QString ruta;

    QFileDialog dialogForDirectory(parent,"seleccione diectorio", inicio, "*" );

    dialogForDirectory.setStyleSheet("QFileDialog { background: yellow; }");

    dialogForDirectory.setWindowFlags(Qt::WindowStaysOnTopHint);
    dialogForDirectory.setFileMode(QFileDialog::Directory);
    dialogForDirectory.setOption(QFileDialog::ShowDirsOnly);

    dialogForDirectory.move(30,10);


    if( dialogForDirectory.exec() == QFileDialog::Accepted)
    {
        //qDebug() << "acepto";
        //qDebug() <<
        ruta = dialogForDirectory.directory().absolutePath();

        if(ruta.contains(" "))
        {
            QMessageBox msgBox;
            msgBox.move(this->pos());
            msgBox.setText(tr("Alerta"));
            msgBox.setInformativeText(QString("Ruta contiene espacios \n %1").arg(ruta));
            msgBox.exec();

            ui->label_trabajando->setText("Ruta contiene espacios");

            return ruta;
        }

    }



    return ruta;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_selecciona_ruta_clicked()
{

    ruta_inicio.clear();

    //ui->textEdit_temas->hide();

    //muestra dialogo del sistema para ubicar la carpeta

    ruta_inicio = dialogo_directorio(QDir::rootPath()+"media/", this);


    //qDebug() << "Ruta : " <<  ruta;

    if( ruta_inicio.isNull() )
    {
        // ui->lineEdit_ruta_app->setText("");
    }

    // si el directorio es valido emite seÃ±al
    if( !ruta_inicio.isNull() )
    {
        //qDebug() << "Ruta : " << ruta;
        ui->lineEdit->setText(ruta_inicio);

        /*
        QFile archivo_mdb(ruta_inicio);


        if(!archivo_mdb.exists())
        {
            QString mensaje;
            mensaje.append(APLICACION_ROCKOLA);
            mensaje.append(" no se encuentra aqui.");

            QMessageBox msgBox;
            msgBox.setText(tr("Alerta"));
            msgBox.setInformativeText(mensaje);
            msgBox.exec();

            ruta_inicio.clear();

            return;
        }
    */


        //ui->pushButton_siguiente->show();

        //qDebug() << ruta;

        //emit dir_Choosed(ruta_inicio);
    }
    else
    {
        return;
    }
}



void MainWindow::corre_find(void)
{
    QString cmd;
    QString output;
    QString strerror;

    //alerta
    if(ruta_inicio.size()==0)
    {
        return;
    }

    lista_audio.clear();
    lista_video.clear();


    ui->label_trabajando->setText("Buscando archivos");

    QCoreApplication::processEvents();

    ruta_inicio = QDir::toNativeSeparators(ruta_inicio);

    cmd = QString("./busca_archivos.sh %1 1").arg(ruta_inicio);

    cmd = QString("/usr/bin/find %1 -iregex \".*\\.\\(mp3\\|wav\\)\"").arg(ruta_inicio);

    qDebug() << cmd;

    QProcess * _proceso = new QProcess(this);

    connect(_proceso, SIGNAL(finished(int,QProcess::ExitStatus)), _proceso, SLOT(deleteLater()));

    _proceso->start(cmd);    // start cmd
    _proceso->waitForFinished(-1);          // wait till done...
    output = _proceso->readAllStandardOutput();     // get std output
    strerror = _proceso->readAllStandardError();    // get std output errors

    qDebug() << "error:  " << strerror;

    lista_audio = output.split("\n");
    lista_audio.removeLast();

    ui->label_trabajando->setText("Trabajando [][]");

    ruta_inicio = QDir::toNativeSeparators(ruta_inicio);

    cmd = QString("/usr/bin/find %1 -iregex \".*\\.\\(wmv\\|mpg\\|avi\\|mpeg\\)\"").arg(ruta_inicio);

    ui->label_trabajando->setText("Trabajando [][][]");

    qDebug() << cmd;

    _proceso->start(cmd);    // start cmd
    _proceso->waitForFinished(-1);          // wait till done...
    output = _proceso->readAllStandardOutput();     // get std output
    strerror = _proceso->readAllStandardError();    // get std output errors

    qDebug() << "error:  " << strerror;

    lista_video = output.split("\n");
    lista_video.removeLast();

    ui->label_trabajando->setText("Trabajando [][][][]");

    //qDebug() << "sqlite: " << output;     // print output
    //qDebug() << "error:  " << strerror;   // print errors

    qDebug() << "lista video " << lista_video.size();
    qDebug() << "lista audio " << lista_audio.size();

    //qDebug() << "lista video " << lista_video;

    ui->label_trabajando->setText("");

}

void MainWindow::cambia_rutas(void)
{   
    QString tmp;
    QString extension;

    archivos_video.clear();
    archivos_audio.clear();

    //modificando la ruta de los archivos

    for(int i=0; i<lista_video.size(); i++)
    {
        tmp = lista_video.at(i);

        Tipo_Archivo_musica T;
        T.ruta_inicial_archivo = tmp;

        //qDebug() << tmp.lastIndexOf(".") << tmp.size() <<  tmp.size() - tmp.lastIndexOf(".");

        extension = tmp.mid(tmp.lastIndexOf("."), tmp.size()-tmp.lastIndexOf("."));

        //qDebug() << "extension: " << extension;

        T.ruta_final_archivo = ruta_inicio + MUSIC_COPIA + fecha.toString("MMMM_dd") + tmp.remove(0, ruta_inicio.size());

        T.ruta_final_archivo.replace(extension, EXTENSIONmp3, Qt::CaseInsensitive);

        //qDebug() << "cambiado: " << T.ruta_final_archivo;
        archivos_video.append(T);
        // falta remover la extension o cambiarla por mp3
    }

    //modificando la ruta de los archivos

    for(int i=0; i<lista_audio.size(); i++)
    {
        tmp = lista_audio.at(i);

        Tipo_Archivo_musica T;
        T.ruta_inicial_archivo = tmp;

        //qDebug() << tmp.lastIndexOf(".") << tmp.size() <<  tmp.size() - tmp.lastIndexOf(".");

        extension = tmp.mid(tmp.lastIndexOf("."), tmp.size()-tmp.lastIndexOf("."));

        //qDebug() << "extension: " << extension;

        T.ruta_final_archivo = ruta_inicio + MUSIC_COPIA + fecha.toString("MMMM_dd") + tmp.remove(0, ruta_inicio.size());

        //qDebug() << "cambiado: " << T.ruta_final_archivo;
        archivos_audio.append(T);
        // falta remover la extension o cambiarla por mp3
    }

}

void MainWindow::lote_proceso(void)
{

    progreso = 0;

    for(int i=0; i< archivos_video.size(); i++ )
    {
        procesar(archivos_video.at(i), TIPO_VIDEO);
    }

    progreso = 0;

    for(int i=0; i< archivos_audio.size(); i++ )
    {
        procesar(archivos_audio.at(i), TIPO_AUDIO);
    }

    //ui->label_trabajando->setText("");
    //ui->label_origen->setText("");
    //ui->label_destino->setText("");

    if(abortar == false)
    {
    ui->label_trabajando->setText("Terminado.");
    ui->label_origen->setText("");
    ui->label_destino->setText("");

    QCoreApplication::processEvents();
    }

}

void MainWindow::procesar(Tipo_Archivo_musica archivo, int tipo_archivo)
{
    QString tmp;
    QString dir_acum;
    QDir DIR;

    if(abortar == true)
    {
        ui->label_trabajando->setText("<<Anulado>>");
        ui->label_origen->setText(archivo.ruta_inicial_archivo);
        ui->label_destino->setText(archivo.ruta_final_archivo);

        QCoreApplication::processEvents();

        return;
    }

    //ya existe el archivo
    if(QFileInfo::exists(archivo.ruta_final_archivo))
    {
        return;
    }

    //qDebug() << "necesito esto: " << archivo.ruta_final_archivo.mid(0 , archivo.ruta_final_archivo.lastIndexOf("/"));

    tmp = archivo.ruta_final_archivo.mid(0 , archivo.ruta_final_archivo.lastIndexOf("/"));

    QStringList directorios = tmp.split("/");

    dir_acum = directorios.at(0);

    for(int i = 1; i < directorios.size(); i++)
    {
        //qDebug() << directorios.at(i);
        dir_acum += "/" + directorios.at(i);
        //qDebug() << dir_acum;
        if(!DIR.exists(dir_acum))
        {
            //qDebug() << "no se pudo crear" <<  ruta_3.at(0)+ruta_3.at(1)+ruta_3.at(2)+ruta_3.at(3);

            if(DIR.mkdir(dir_acum))
            {
                //qDebug() << "creado: " << dir_acum;

                ui->label_trabajando->setText(QString("Creando directorio: %1").arg(dir_acum));

                QCoreApplication::processEvents();
            }
        }
    }

    QString cmd;

    QString trabajando;

    switch (tipo_archivo)
    {
    case TIPO_VIDEO:
        cmd = QString("/usr/bin/ffmpeg -nostats -loglevel quiet  -y -i \"%1\" -acodec libmp3lame -b:a 160k -ac 2 -ar 44100 \"%2\"").arg(archivo.ruta_inicial_archivo, archivo.ruta_final_archivo);
        trabajando = QString("Convirtiendo archivos de video: %1 de %2 ").arg(progreso).arg(lista_video.size());
        break;

    case TIPO_AUDIO:
        cmd = QString("/bin/cp \"%1\" \"%2\"").arg(archivo.ruta_inicial_archivo, archivo.ruta_final_archivo);
        trabajando = QString("Copiando archivos de audio: %1 de %2 ").arg(progreso).arg(lista_audio.size());
        break;

    default:
        return;
        break;
    }



    //qDebug() << cmd;


    /*
    QProcess * _proceso = new QProcess(this);
    connect(_proceso, SIGNAL(finished(int,QProcess::ExitStatus)), _proceso, SLOT(deleteLater()));
    //connect(_proceso, SIGNAL(readyReadStandardOutput()), this, SLOT(actualiza_progreso()));
    _proceso->start(cmd);    // start cmd
    _proceso->waitForFinished(-1);          // wait till done...
    */



    if(progreso%2) trabajando.append("/");
    else
    if(progreso%3) trabajando.append("-");
    else
    if(progreso%5) trabajando.append("\\");
    else
    if(progreso%7) trabajando.append("|");

    ui->label_trabajando->setText(trabajando);

    ui->label_origen->setText(archivo.ruta_inicial_archivo);
    ui->label_destino->setText(archivo.ruta_final_archivo);

    QCoreApplication::processEvents();


    QProcess::execute(cmd);

    progreso++;

    //output = _proceso->readAllStandardOutput();     // get std output
    //strerror = _proceso->readAllStandardError();    // get std output errors

    QCoreApplication::processEvents();

    //xxxxx detener app

}

void MainWindow::on_pushButton_inicia_clicked()
{
    corre_find();
    cambia_rutas();
    lote_proceso();
}

void MainWindow::on_pushButton_abortar_clicked()
{
    abortar = true;

    QProcess * _proceso = new QProcess(this);

    QString cmd = QString("killall ffmpeg");

    _proceso->start(cmd);

    _proceso->waitForFinished(-1);

    qDebug() << "killall: " << _proceso->readAllStandardOutput();
}
