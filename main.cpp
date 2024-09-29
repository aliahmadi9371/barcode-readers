#include "mainwidget.h"
#include <QApplication>
#include </usr/include/stdio.h>
#include </usr/include/stdlib.h>
#include <QDateTime>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QHash>

#define LOGSIZE 1024 * 100000
#ifdef QT_DEBUG
   #define LOG_TO_FILE true
#else
  #define LOG_TO_FILE true
#endif

void myMessageOutput(QtMsgType type,
                     const QMessageLogContext &context,
                     const QString &msg)
{
//    QHash<QtMsgType, QString> msgLevelHash({{QtDebugMsg, "Debug"}, {QtInfoMsg, "Info"}, {QtWarningMsg, "Warning"}, {QtCriticalMsg, "Critical"}, {QtFatalMsg, "Fatal"}});
    QString logLevelName;

    switch(type){
       case QtDebugMsg:{
           logLevelName = "Debug";
           break;
       }
       case QtInfoMsg:{
         logLevelName = "Info";
         break;
       }
       case QtWarningMsg:{
              logLevelName = "Warning";
              break;
       }
       case QtCriticalMsg:{
              logLevelName = "Critical";
              break;
       }
       case QtFatalMsg:{
          logLevelName = "Fatal";
          break;
       }
       default:{
          logLevelName = "Other";
          break;
       }
    }

    QByteArray localMsg = msg.toLocal8Bit();
    QDateTime time = QDateTime::currentDateTime();
    QString formattedTime = time.toString("MM-dd hh:mm:ss.zzz");
    QString fileName =  QString("log/Log_%1_.txt")
           .arg(QDate::currentDate().toString("yyyy_MM_dd"));
    QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
    QByteArray logLevelMsg = logLevelName.toLocal8Bit();
    QDir dir;
    if (!dir.exists("log"))
       dir.mkpath("log");
    QFile outFileCheck(fileName);
    QString NewFileName = fileName;
    int size = static_cast<int>(outFileCheck.size());
    int counter = 0 ;
    while ( size > LOGSIZE ){
       counter++;
       NewFileName = fileName+QString::number(counter);
       QFile outFileCheck(NewFileName);
       size = static_cast<int>(outFileCheck.size());
    }
    if (LOG_TO_FILE) {
       QString txt = QString("%1 %2: %3 (%4)").arg(formattedTime, logLevelName, msg,  context.file);
       QFile outFile(NewFileName);
       outFile.open(QIODevice::WriteOnly | QIODevice::Append);
       QTextStream ts(&outFile);
       ts << txt << endl;
       outFile.close();
    } else {
       fprintf(stderr, "%s %s: %s (%s:%u, %s)\n", formattedTimeMsg.constData(), logLevelMsg.constData(), localMsg.constData(), context.file, context.line, context.function);
       fflush(stderr);
    }
    if (type == QtFatalMsg)
       abort();
}


int main(int argc, char *argv[])
{

    QSettings setting("./config.ini",QSettings::IniFormat);
    int j = setting.value("number/readBarcode",0).toInt();
    bool bDebug = setting.value("SoftWare/debug",true).toBool();

    if(j>250){
        j = 0;
        setting.setValue("number/readBarcode",0);
    }

    qDebug() << "Debug to file: " << bDebug;
        if(bDebug)
            qInstallMessageHandler(myMessageOutput);


    qDebug() << "--------------------------------------------------";
    qDebug() << "--------------------------------------------------";
    qDebug() << "--------------------App Start---------------------";
    qDebug() << "--------------------------------------------------";
    qDebug() << "--------------------------------------------------";
    qDebug() << "";

    QApplication a(argc, argv);
    MainWidget w;
    w.show();

    return a.exec();
}
