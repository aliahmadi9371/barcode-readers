#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QSettings>
#include <QSocketNotifier>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomElement>
#include <QtXml>
#include <QXmlStreamReader>
#include <QDateTime>
#include <QApplication>

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"
#include "/usr/include/linux/input.h"
#include "fcntl.h"
#include "/usr/include/linux/input-event-codes.h"

#include <QUrl>
#include <QUrlQuery>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>



#define KEY_RELEASE 0
#define KEY_PRESS 1
#define KEY_KEEPING_PRESSED 2

#define OGATE1 31
#define OGATE2 33
#define OFAN 35
#define IINPUT 37

//#define MYDEBUG

#define UNKNOWN_KEY "\0"
#define UK UNKNOWN_KEY

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

#define ASSERT_ON_COMPILE(expn) typedef char __C_ASSERT__[(expn) ? 1 : -1]


/*
 * The order of the keys is defined in linux/input.h
 */
static char *key_names[] = {
   UK, "<ESC>",
   "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
   "<Backspace>", "<Tab>",
   "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
   "[", "]", "<Enter>", "<LCtrl>",
   "a", "s", "d", "f", "g", "h", "j", "k", "l", ";",
   "'", "`", "<LShift>",
   "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/",
   "<RShift>",
   "<KP*>",
   "<LAlt>", " ", "<CapsLock>",
   "<F1>", "<F2>", "<F3>", "<F4>", "<F5>", "<Fin>", "<F7>", "<Fout>", "<F9>", "<F10>",
   "<NumLock>", "<ScrollLock>",
   "<KP7>", "<KPout>", "<KP9>",
   "<KP->",
   "<KP4>", "<KP5>", "<KPin>",
   "<KP+>",
   "<KP1>", "<KP2>", "<KP3>", "<KP0>",
   "<KP.>",
   UK, UK, UK,
   "<F11>", "<F12>",
   UK, UK, UK, UK, UK, UK, UK,
   "<KPEnter>", "<RCtrl>", "<KP/>", "<SysRq>", "<RAlt>", UK,
   "<Home>", "<Up>", "<PageUp>", "<Left>", "<Right>", "<End>", "<Down>",
   "<PageDown>", "<Insert>", "<Delete>"
};

static char *shift_key_names[] = {
   UK, "<ESC>",
   "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+",
   "<Backspace>", "<Tab>",
   "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
   "{", "}", "<Enter>", "<LCtrl>",
   "A", "S", "D", "F", "G", "H", "J", "K", "L", ":",
   "\"", "~", "<LShift>",
   "|", "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?",
   "<RShift>",
   "<KP*>",
   "<LAlt>", " ", "<CapsLock>",
   "<F1>", "<F2>", "<F3>", "<F4>", "<F5>", "<Fin>", "<F7>", "<Fout>", "<F9>", "<F10>",
   "<NumLock>", "<ScrollLock>",
   "<KP7>", "<KPout>", "<KP9>",
   "<KP->",
   "<KP4>", "<KP5>", "<KPin>",
   "<KP+>",
   "<KP1>", "<KP2>", "<KP3>", "<KP0>",
   "<KP.>",
   UK, UK, UK,
   "<F11>", "<F12>",
   UK, UK, UK, UK, UK, UK, UK,
   "<KPEnter>", "<RCtrl>", "<KP/>", "<SysRq>", "<RAlt>", UK,
   "<Home>", "<Up>", "<PageUp>", "<Left>", "<Right>", "<End>", "<Down>",
   "<PageDown>", "<Insert>", "<Delete>"
};

#undef UK


namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    bool isShift(unsigned short code);
    char* getKeychar(unsigned short code, bool shift_pressed);

protected:

    //QTimer tSetting;

    bool in_proc;
    int readBarcode;
    QString IniPath;

    QString fileName_in;
    int fd_in;
    QSocketNotifier *notifier_in;
    QString bufCodes_in;
    bool shiftpressed_in;


    QString fileName_out;
    int fd_out;
    QSocketNotifier *notifier_out;
    QString bufCodes_out;
    bool shiftpressed_out;

    QString SoapURL;
    QString userName;
    QString passWord;
    int id;
    int iGateTime;

    QNetworkAccessManager *managerIn;
    QNetworkAccessManager *managerOut;

    QNetworkReply *reply_In;
    QTimer timer_In;
    QTime time_In;
    int inTimeout;

    QNetworkReply *reply_Out;
    QTimer timer_out;
    QTime time_Out;
    int outTimeout;

    QNetworkRequest requestIn;
    QNetworkRequest requestOut;

    QTimer tgateTimerIn;
    QTimer tgateTimerOut;



    //variables that responsible for json req and reply


    bool abort ;

    void setRequestHeaders_In(QString url);
    void setRequestHeaders_Out(QString url);

    void SendRequest(QString query, int iManagers);
    QString prepareReq(QString user, QString pass, QString barcode, int id, int io);

    void InitValues();
    void GetReadersInfo();
    void CreateObjects();
    void Open_HidDevice();
    void Close_HidDevice();
    void setWiringOPI();
    void connection_all();
    void ConfigApplication();

    void ReadBarcode_In(QString barcode);
    void ReadBarcode_Out(QString barcode);

    void checkPermissionIn(int iOut);
    void checkPermissionOut(int iOut);

    void notifierSetEnable(bool enable);

    void saveReadBarcode();

    void ResetReading(bool InOut);

    //functions hat responsible for json req and reply :

    void PostReqOut(QString barcode);
    void PostReqIn(QString barcode);
    void connections();
    void ReadSettings();

private:
    Ui::MainWidget *ui;

signals:
    void InputDetected();


public slots:
    void hanlde_readNotification_In(int socket);
    void hanlde_readNotification_Out(int socket);

    void recieveInput();

    void ReplyFinished_In(QNetworkReply* reply);
    void ReplyFinished_Out(QNetworkReply* reply);

    void Reply_In_timeOut();
    void Reply_Out_timeOut();

    //slots responsible for Json req and reply

    bool parse_function_status( QByteArray replyedData );
    bool parse_function_data( QByteArray replyedData );

};

#endif // MAINWIDGET_H
