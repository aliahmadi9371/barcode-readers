#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QtNetwork>
#include <QLabel>
#include <wiringPi.h>
#include "filerotationlogsink.h"




#define OGATE1 31
#define OGATE2 33
#define OFAN 35
#define IINPUT 37


namespace Ui {
class mainForm;
}

class mainForm : public QWidget
{
    Q_OBJECT


public:
    explicit mainForm(QWidget *parent = 0);
    ~mainForm();

private:
    Ui::mainForm *ui;
    QTimer tGateTimer;

    libLogging::FileRotationLogSink *log;
    libLogging::FileRotationLogSink *mainlog;

    QString scanner;
    QPixmap pnBG;
    QPixmap psBG;
    QPixmap pfBG;

    QPixmap pIOO;
    QPixmap pIOF;
    QPixmap pINO;
    QPixmap pINF;
    QPixmap pISO;
    QPixmap pISF;
    QPixmap pILogo;

    QLabel *lblMainImg;
    QLabel *lblLogoImg;
    QLabel *lblSrvImg;
    QLabel *lblNetImg;
    QLabel *lblAllImg;
    QLabel *lblIP;
    QLabel *lblTimes;
    QLabel *lblMsg;

    QString cNColor ;
    QString cOColor ;
    QString cFColor ;

    QTime tTi;
    QTime tWe;
    QTime tRe;

    int iTi;
    int iWe;
    int iRe;
    int iSST;
    int iNST;

    QString sInitTxt ;
    QString sWaitTxt;

    QString sServerIP;
    QString sServerPort;
    QString sBaseUrl;
    int iGateType;
    QString sPassword;
    QString sUserName;
    int iDeviceID;
    int iRelayTimeout;

    void getserverresp(QString inp);
    void drawdisp(int iSt,QString sMsg );
    QString getcurrentIP();


protected:
    void keyPressEvent( QKeyEvent *event );    
    QNetworkAccessManager *myNetworkAccessManager;

public slots:
    void replyFinished(QNetworkReply *reply);
    void recieveInput();

 signals:
    void InputDetected();
};

#endif // MAINFORM_H
