#ifndef DEFINICIONES
#define DEFINICIONES

//#include <QList>
#include <QString>

const QString MUSIC_COPIA("SOLOmp3_");

const QString EXTENSIONmp3(".mp3");

const QString FFMPEG("/usr/bin/ffmpeg");

const QString BUSCA("./busca_archivos.sh");

const QString myStilo("QLineEdit { \
                padding: 1px; \
                border-style: solid; \
                border: 2px solid gray; \
                border-radius: 8px; \
                } \
                 \
                QLabel { \
                font-weight: bold; \
                font-size: 12px; \
                } \
                 \
                QPushButton { \
                background-color: rgb(234, 231, 255); \
                border-width: 1px; \
                border-color: #339; \
                border-style: solid; \
                border-radius: 7; \
                padding: 3px; \
                font-size: 10px; \
                font: bold large \"Arial\"; \
                padding-left: 5px; \
                padding-right: 5px; \
                min-width: 50px; \
                min-height: 13px; \
                } \
                \
                QProgressBar { \
                border-color: rgb(85, 170, 255); \
                border-bottom-right-radius: 1px; \
                border-bottom-left-radius: 1px; \
                border: 1px solid black; \
                } \
                 \
                QProgressBar::chunk { \
                background-color: qlineargradient(spread:pad, x1:0.064, y1:0.590909, x2:1, y2:0.592, stop:0 rgba(0, 184, 119, 255), stop:1 rgba(243, 243, 121, 255)); \
                border-bottom-right-radius: 1px; \
                border-bottom-left-radius: 1px; \
                border: 1px solid black; \
                } \
                  \
                ");

const int TIPO_VIDEO = 10;
const int TIPO_AUDIO = 14;

class Tipo_Archivo_musica
{
    void tipo_Archivo_musica(void)
    {

    }


public:
    QString ruta_inicial_archivo;
    QString ruta_final_archivo;


};


#endif // DEFINICIONES

