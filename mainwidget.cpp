#include "mainwidget.h"
#include "ui_mainwidget.h"

static MainWidget* main = NULL;

void readiput(){
    if(main)
        emit main->InputDetected();
}
//---------------------------------------------------------------------------------------------------------------

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);


    //get ini settings
    IniPath = "./config.ini";
    InitValues();
    GetReadersInfo();
    CreateObjects();
    ConfigApplication();
    connection_all();
    Open_HidDevice();
    setWiringOPI();
}
//---------------------------------------------------------------------------------------------------------------

MainWidget::~MainWidget()
{
    qDebug() <<  "App close";
    qDebug() << "--------------------------------------------------";
    qDebug() << "--------------------------------------------------";

    saveReadBarcode();

    digitalWrite(OGATE1,0);
    digitalWrite(OGATE2,0);

    if(fd_in > 0) ::close(fd_in); // qwidgwet::close();
    if(fd_out > 0) ::close(fd_out); // qwidgwet::close();
    managerIn->deleteLater();
    managerOut->deleteLater();

    if(reply_In->isOpen())
        reply_In->deleteLater();
    if(reply_Out->isOpen())
        reply_Out->deleteLater();
    delete ui;
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::InitValues()
{
    reply_In = NULL;
    reply_Out = NULL;

    notifier_in = NULL;
    notifier_out = NULL;

    abort = false;

    QSettings setting(IniPath,QSettings::IniFormat);
    readBarcode = setting.value("number/readBarcode",0).toInt();

    bufCodes_in = "";
    bufCodes_out = "";

    shiftpressed_in = false;
    shiftpressed_out = false;

    in_proc = false;


}

//---------------------------------------------------------------------------------------------------------------

//tools
void MainWidget::GetReadersInfo()
{
    QSettings setting(IniPath,QSettings::IniFormat);
    int devEvIn = setting.value("IO/enterance", 6).toInt();
    qDebug() << "##################################################";
    qDebug() << "devEvIn = " << devEvIn;
    switch(devEvIn){
        case 6:fileName_in = setting.value("BarcodeHw/usb6", "/dev/event3").toString();
            break;
        case 7:fileName_in = setting.value("BarcodeHw/usb7", "/dev/event4").toString();
            break;
        case 8:fileName_in = setting.value("BarcodeHw/usb8", "/dev/event5").toString();
            break;
    }
    int devEvout = setting.value("IO/exit", 8).toInt();
    qDebug() << "devEvout = " << devEvout;
    switch(devEvout){
        case 6:fileName_out = setting.value("BarcodeHw/usb6", "/dev/event3").toString();
            break;
        case 7:fileName_out = setting.value("BarcodeHw/usb7", "/dev/event4").toString();
            break;
        case 8:fileName_out = setting.value("BarcodeHw/usb8", "/dev/event5").toString();
            break;
    }
    SoapURL = setting.value("Server/soapurl","http://81.16.124.168/visitor/post").toString();
    userName = setting.value("Info/user","admin").toString();
    passWord = setting.value("Info/pass","12345").toString();
    id = setting.value("Info/id",0).toInt();
    inTimeout = setting.value("SoftWare/tReplyIn",1000).toInt();
    outTimeout = setting.value("SoftWare/tReplyOut",1000).toInt();
    iGateTime = setting.value("SoftWare/tGateOpen",8000).toInt();


    qDebug() << "get setting and config reader";
    qDebug() << "fileName_in : " << fileName_in;
    qDebug() << "fileName_out : " << fileName_out;
    qDebug() << "soap url: " << SoapURL;
    qDebug() << "username: " << userName;
    qDebug() << "pass: " << passWord;
    qDebug() << "id: " << id;
    qDebug() << "reply in time: " << inTimeout;
    qDebug() << "reply out time: " << outTimeout;
    qDebug() << "gate open time: " << iGateTime;
    qDebug() << "##################################################";
    qDebug() << "";
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::ConfigApplication()
{
    main = this;

    timer_In.setSingleShot(true);
    timer_out.setSingleShot(true);

    tgateTimerIn.setSingleShot(true);
    tgateTimerOut.setSingleShot(true);
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::CreateObjects()
{
    managerIn = new QNetworkAccessManager(this);
    managerOut = new QNetworkAccessManager(this);
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::connection_all()
{

    connect(managerIn, SIGNAL(finished(QNetworkReply*)), this, SLOT(ReplyFinished_In(QNetworkReply*)), Qt::QueuedConnection);
    connect(managerOut, SIGNAL(finished(QNetworkReply*)), this, SLOT(ReplyFinished_Out(QNetworkReply*)), Qt::QueuedConnection);

    connect(&timer_In,SIGNAL(timeout()),this,SLOT(Reply_In_timeOut()), Qt::QueuedConnection);
    connect(&timer_out,SIGNAL(timeout()),this,SLOT(Reply_Out_timeOut()), Qt::QueuedConnection);

    connect(&tgateTimerIn,SIGNAL(timeout()),this,SLOT(recieveInput()), Qt::QueuedConnection);
    connect(&tgateTimerOut,SIGNAL(timeout()),this,SLOT(recieveInput()), Qt::QueuedConnection);

}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::Open_HidDevice()
{
    //open hid device based on ini settings
    QFile file_in;
    QFile file_out;
    file_in.setFileName(fileName_in);
    file_out.setFileName(fileName_out);

    if(!file_in.exists()){
        qDebug() << fileName_in << " : file does not exist : INPUT Barcode Reader was not found";
        //return;
    }
    else{
        fd_in = open(fileName_in.toLocal8Bit().data(),O_RDONLY | O_NONBLOCK);
        if(fd_in == -1){
            qDebug() << fileName_in << " : can not open file";
            return;
        }
        else{
            notifier_in = new QSocketNotifier(fd_in,QSocketNotifier::Read,this);
            connect(notifier_in,&QSocketNotifier::activated,this,&MainWidget::hanlde_readNotification_In);
        }
    }


    if(!file_out.exists()){
        qDebug() << fileName_out << " : file does not exist : OUPUT Barcode Reader was not found";
        //return;
    }
    else{
        fd_out = open(fileName_out.toLocal8Bit().data(),O_RDONLY | O_NONBLOCK);
        if(fd_out == -1){
            qDebug() << fileName_out << " : can not open file";
            return;
        }
        else{
            notifier_out = new QSocketNotifier(fd_out,QSocketNotifier::Read,this);
            connect(notifier_out,&QSocketNotifier::activated,this,&MainWidget::hanlde_readNotification_Out);
        }
    }
    //notifierSetEnable(true);
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::Close_HidDevice()
{
    if(fd_in > 0){
        ::close(fd_in);
    }

    if(fd_out > 0){
        ::close(fd_out);
    }
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::notifierSetEnable(bool enable)
{
    if(enable == true){
        Open_HidDevice();
        //notifier_in->setEnabled(true);
        //notifier_out->setEnabled(true);

//        connect(notifier_in,&QSocketNotifier::activated,this,&MainWidget::hanlde_readNotification_In);
//        connect(notifier_out,&QSocketNotifier::activated,this,&MainWidget::hanlde_readNotification_Out);
        qDebug() << "notifierSetEnable : true";
        qDebug() << "########################################################################";
    }
    else{
        Close_HidDevice();
        if(notifier_in){
        notifier_in->setEnabled(false);
            delete notifier_in;
            notifier_in = NULL;
        }
        if(notifier_out){
        notifier_out->setEnabled(false);
            delete notifier_out;
            notifier_out = NULL;
        }

        qDebug() << "notifierSetEnable : false";

//        disconnect(notifier_in,&QSocketNotifier::activated,this,&MainWidget::hanlde_readNotification_In);
//        disconnect(notifier_out,&QSocketNotifier::activated,this,&MainWidget::hanlde_readNotification_Out);
    }



}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::setWiringOPI()
{
    wiringPiSetupPhys();
    pinMode(OGATE1,OUTPUT);
    pinMode(OGATE2,OUTPUT);
    pinMode(OFAN,OUTPUT);
    pinMode(IINPUT,INPUT);
    pullUpDnControl(IINPUT, PUD_DOWN);

    wiringPiISR(IINPUT, INT_EDGE_RISING, &readiput);
    connect(this,SIGNAL(InputDetected()), this, SLOT(recieveInput()));
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::recieveInput()
{
    qDebug() << "end proc";
    tgateTimerIn.stop();
    tgateTimerOut.stop();

    digitalWrite(OGATE1,0);
    digitalWrite(OGATE2,0);

    notifierSetEnable(true);
    saveReadBarcode();
    in_proc = false;
}
//---------------------------------------------------------------------------------------------------------------\

void MainWidget::hanlde_readNotification_In(int)
{
    if(in_proc){
        //qDebug() << "in proc";
        //bufCodes_in = "";
        //bufCodes_out = "";
        return;
    }
    struct input_event buf;
    while(read(fd_in,&buf,sizeof(buf)) > 0){
        if(buf.value == KEY_PRESS && buf.type == EV_KEY){
            if(bufCodes_in == "")
                time_In.restart();
            if(buf.code == KEY_ENTER){
                qInfo() << "barcode read from USB IN: " << bufCodes_in;
                readBarcode++;
                qInfo() << "number of read barcodes: " <<  readBarcode;
                qInfo() << "read input barcode time: " << time_In.elapsed() << " ms";
                QString barcode = bufCodes_in;
                bufCodes_in = "";
                ReadBarcode_In(barcode);
                return;
            }
            if(isShift(buf.code) == true){
                shiftpressed_in = true;
                return;

            }
            else{
               bufCodes_in = bufCodes_in + getKeychar(buf.code, shiftpressed_in);
               shiftpressed_in = false;
               return;
            }
        }
    }
}


//---------------------------------------------------------------------------------------------------------------

void MainWidget::hanlde_readNotification_Out(int)
{
    if(in_proc){
        //qDebug() << "in proc";
        //bufCodes_in = "";
        //bufCodes_out = "";
        return;
    }
    struct input_event buf;
    while(read(fd_out,&buf,sizeof(buf)) > 0){
        if(buf.value == KEY_PRESS && buf.type == EV_KEY){
            if(bufCodes_out == "")
                time_Out.restart();
            if(buf.code == KEY_ENTER){
                qInfo() << "barcode read from USB OUT: " << bufCodes_out;
                readBarcode++;
                qInfo() << "number of read barcodes: " <<  readBarcode;
                qInfo() << "read output barcode time: " << time_Out.elapsed() << " ms";
                QString barcode = bufCodes_out;
                bufCodes_out = "";
                ReadBarcode_Out(barcode);
                return;
            }
            if(isShift(buf.code) == true){
                shiftpressed_out = true;
                return;
            }
            else{
                bufCodes_out = bufCodes_out + getKeychar(buf.code, shiftpressed_out);
                shiftpressed_out = false;
                return;
            }
        }
    }
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::ReadBarcode_In(QString barcode)
{
    in_proc = true;
    notifierSetEnable(false);
    qDebug() << "send Post request in";
    time_In.restart();
    PostReqIn(barcode);
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::ReadBarcode_Out(QString barcode)
{
    in_proc = true;
    notifierSetEnable(false);
    qDebug() << "send post request out";
    time_Out.restart();
    PostReqOut(barcode);
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::ReplyFinished_In(QNetworkReply* reply)
{
    qDebug() << "ReplyFinished_In : abort : " << abort;
    timer_In.stop();
    reply_In = reply;
    if(abort){
        qDebug()<< "ReplyFinished_In : Aborted : " << reply_In->errorString();
        ResetReading(true);
    }
    else{
        qDebug()<< "ReplyFinished_In : Here is Reply : ";
        QByteArray replyedData = reply_In->readAll();
        qDebug()<< replyedData;
        if(parse_function_status(replyedData)){
            if(parse_function_data(replyedData)){
                checkPermissionIn(1);
            }
            else{
                qDebug() << " : invalid  data returned value server";
                ResetReading(true);
            }
        }
        else{
            qDebug() << "ReplyFinished_In : invalid status returned value server";
            ResetReading(true);
        }
    }
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::ReplyFinished_Out(QNetworkReply* reply)
{
    qDebug() << "ReplyFinished_Out : abort : " << abort;
    timer_out.stop();
    reply_Out = reply;
    if (abort){
        qDebug()<< "ReplyFinished_Out : Aborted : " << reply_Out->errorString();
        ResetReading(false);
    }
    else{
        qDebug()<< "ReplyFinished_Out : Here is Reply : ";
        QByteArray replyedData = reply_Out->readAll();
        qDebug()<< replyedData;
        if(parse_function_status(replyedData)){
            if(parse_function_data(replyedData)){
                checkPermissionOut(1);
            }
            else{
                qDebug() << "ReplyFinished_Out : invalid  data returned value server";
                ResetReading(false);
            }
        }
        else{
            qDebug() << "ReplyFinished_Out : invalid status returned value server";
            ResetReading(false);
        }
    }
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::Reply_In_timeOut()
{
    abort = true;
    reply_In->abort();
    qDebug() << "Reply_timeOut_In : exterance Reply timeout";
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::Reply_Out_timeOut()
{
    abort = true;
    reply_Out->abort();
    qDebug() << "Reply_timeOut_Out : exterance Reply timeout";
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::checkPermissionIn(int iOut)
{
    if(iOut == 1){
        tgateTimerIn.start(iGateTime);
        qDebug() << "valid permission" <<  " | " <<"Open Gate from the enterance";
        digitalWrite(OGATE1,1);
    }
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::checkPermissionOut(int iOut)
{
    if(iOut == 1){
        tgateTimerOut.start(iGateTime);
        qDebug() << "valid permission" <<  " | " <<"Open Gate from the exit";
        digitalWrite(OGATE2,1);
    }
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::ResetReading(bool InOut)
{
    qDebug() << "ResetReading : in/out" << InOut;
    if(InOut){
        digitalWrite(OGATE1,0);
    }
    else{
        digitalWrite(OGATE2,0);
    }

    in_proc = false;
    notifierSetEnable(true);
    saveReadBarcode();
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::PostReqIn(QString barcode)
{
    abort =false ;
    QUrl url(SoapURL);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray arr = barcode.toLocal8Bit();
    char *barcode_data = arr.data();
    QJsonObject json;
    json.insert("codekart", barcode_data);
    json.insert("gate","12000030");

    qDebug() << "PostReqIn : ";
    for(int i=0; i<json.count(); i++)
        qDebug() << i+1 << " - {key : " << json.keys().at(i) << " , value : "<< json.value(json.keys().at(i)).toString() << "}";

    json.insert("codekart", barcode_data);
    timer_In.start(inTimeout);
    reply_In = managerIn->post(request, QJsonDocument(json).toJson());

}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::PostReqOut(QString barcode)
{
    abort =false ;
    QUrl url(SoapURL);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray arr = barcode.toLocal8Bit();
    char *barcode_data=arr.data();
    QJsonObject json;
    json.insert("codekart", barcode_data);
    json.insert("gate","12000031");

    qDebug() << "PostReqOut : ";
    for(int i=0; i<json.count(); i++)
        qDebug() << i+1 << " - {key : " << json.keys().at(i) << " , value : "<< json.value(json.keys().at(i)).toString() << "}";

    timer_out.start(outTimeout);
    reply_Out = managerOut->post(request, QJsonDocument(json).toJson());
}
//---------------------------------------------------------------------------------------------------------------

bool MainWidget::parse_function_status(QByteArray replyedData)
{

    // Attention this function only parse the DATA in replyed json  .
    bool ret = false ;
    QJsonDocument _json_reply = QJsonDocument::fromJson(replyedData);

    if (!_json_reply.isObject()){
        qDebug() << "Document is not an object";
    }
    else{
        qDebug() << "Document is an object";
        QJsonObject JsonObj = _json_reply.object();
        QJsonValue jsonValue = JsonObj.value("status");
        if(jsonValue.isUndefined()){
            qDebug() << "Key id does not exist";
        }
        else{
            QString status = jsonValue.toString();
            qDebug() << jsonValue.toString();
            if( status == "200")
                ret = true;
       }
    }
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------

bool MainWidget::parse_function_data(QByteArray replyedData)
{
    bool ret = false ;
    QJsonDocument _json_reply = QJsonDocument::fromJson(replyedData);
    if (!_json_reply.isObject())
        qDebug() << "Document is not an object";
    else{
        qDebug() << "Document is an object";
        QJsonObject JsonObj = _json_reply.object();
        QJsonValue jsonValue = JsonObj.value("data");
        if(jsonValue.isUndefined())
            qDebug() << "Key data is not false";
        else{
            QString data = jsonValue.toString();
            qDebug() << jsonValue.toString();
            if (data == "true")
                ret = true;
        }
    }
    return ret;
 }

//---------------------------------------------------------------------------------------------------------------------

//keycode to ascii-code
bool MainWidget::isShift(unsigned short code)
{
    if(code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------------------------------------------

char* MainWidget::getKeychar(unsigned short code, bool shift_pressed)
{
    ASSERT_ON_COMPILE(ARRAY_SIZE(key_names) == ARRAY_SIZE(shift_key_names));

       // LOG("%s", shift_pressed ? "true" : "false");
       char **arr;
       if (shift_pressed != 0) {
          arr = shift_key_names;
       } else {
          arr = key_names;
       }

       if (code < ARRAY_SIZE(key_names)) {
          return arr[code];
       } else {

          return UNKNOWN_KEY;
       }
}
//---------------------------------------------------------------------------------------------------------------

void MainWidget::saveReadBarcode()
{
    QSettings set(IniPath,QSettings::IniFormat);
    set.setValue("number/readBarcode",readBarcode);

    if (readBarcode > 250){
        QApplication::quit();

    }

}
//---------------------------------------------------------------------------------------------------------------

