#include "mainform.h"
#include "ui_mainform.h"



static mainForm* mainfrm = NULL;



void readiput(){
    if (mainfrm)
        emit mainfrm->InputDetected();
}

mainForm::mainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainForm)
{
    ui->setupUi(this);
    myNetworkAccessManager = new QNetworkAccessManager(this);
    connect(myNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this,
                     SLOT(replyFinished(QNetworkReply*)));
    connect(&tGateTimer, SIGNAL(timeout()), this, SLOT(recieveInput()));
    mainfrm = this;

    log = new libLogging::FileRotationLogSink(1000,"/var/miladtower/log","/var/miladtower/log.bk");
    mainlog = new libLogging::FileRotationLogSink(20000,"/var/miladtower/mainlog","/var/miladtower/mainlog.bk");


    log->GetCurrentStream() << "appstart" << endl;


    wiringPiSetupPhys();
    pinMode(OGATE1,OUTPUT);
    pinMode(OGATE2,OUTPUT);
    pinMode(OFAN,OUTPUT);
    pinMode(IINPUT,INPUT);
    pullUpDnControl(IINPUT, PUD_DOWN);

    wiringPiISR(IINPUT, INT_EDGE_RISING, &readiput);
    connect(this,SIGNAL(InputDetected()), this, SLOT(recieveInput()));



    pnBG.load("/var/miladtower/img/b1.png","png");
    psBG.load("/var/miladtower/img/b2.png","png");
    pfBG.load("/var/miladtower/img/b3.png","png");
    pILogo.load("/var/miladtower/img/l.png","png");
    pIOO.load("/var/miladtower/img/o2.png","png");
    pIOF.load("/var/miladtower/img/o1.png","png");
    pINO.load("/var/miladtower/img/n2.png","png");
    pINF.load("/var/miladtower/img/n1.png","png");
    pISO.load("/var/miladtower/img/s2.png","png");
    pISF.load("/var/miladtower/img/s1.png","png");

    cNColor = "#0084a5";
    cOColor = "#00bc63";
    cFColor = "#9f2901";

    iTi = 0 ;
    iWe = 0 ;
    iRe = 0 ;
    iSST = 1 ;
    iNST = 1 ;

    scanner = "";

    QSettings setting(QString("/var/miladtower/config.ini"),QSettings::IniFormat);
    sServerIP =setting.value("server/srv","31.24.237.161").toString();
    sServerPort =setting.value("server/port","86").toString();
    sBaseUrl =setting.value("server/baseurl","").toString();
    iGateType =setting.value("urlinfo/gatetype", 2).toInt();
    sPassword =setting.value("urlinfo/password","").toString();
    sUserName =setting.value("urlinfo/username","").toString();
    iDeviceID =setting.value("urlinfo/deviceid",102).toInt();
    iRelayTimeout =setting.value("urlinfo/relay",8).toInt()*1000;



    QRect qbmain(0, (480-330), 800, 249);
    QRect qblogo(10,10,187,142);
    QRect qbIO(800-20-66, 20, 66, 66);
    QRect qbIN(800-2*20-2*66, 20, 66, 66);
    QRect qbIS(800-3*20-3*66, 20, 66, 66);
    QRect qbtext(52, 165, 370, 180);
    QRect qbTimes(800 - 3*50, 450, 300,30);
    QRect qbIp(10, 450, 300,30);

    lblMainImg = new QLabel(this);
    lblMainImg->setGeometry(qbmain);

    lblLogoImg = new QLabel(this);
    lblLogoImg->setGeometry(qblogo);


    lblSrvImg = new QLabel(this);
    lblSrvImg->setGeometry(qbIS);


    lblNetImg = new QLabel(this);
    lblNetImg->setGeometry(qbIN);


    lblAllImg = new QLabel(this);
    lblAllImg->setGeometry(qbIO);

    lblTimes = new QLabel(this);
    lblTimes->setGeometry(qbTimes);

    lblIP = new QLabel(this);
    lblIP->setGeometry(qbIp);

    lblMsg = new QLabel(this);
    lblMsg->setGeometry(qbtext);
    QFont f( "B Yekan", 24, QFont::Bold);
    lblMsg->setFont(f);
    lblMsg->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    lblMsg->setWordWrap(true);
    sInitTxt = "لطفا بلیط را مقابل بارکدخوان قرار دهید." ;
    sWaitTxt = "لطفا منتظر بمانید." ;

    this->drawdisp(0,sInitTxt);
}


mainForm::~mainForm()
{
    lblMainImg->deleteLater();
    lblLogoImg->deleteLater();
    lblSrvImg->deleteLater();
    lblNetImg->deleteLater();
    lblAllImg->deleteLater();
    lblIP->deleteLater();
    lblTimes->deleteLater();
    lblMsg->deleteLater();
    myNetworkAccessManager->deleteLater();
    delete mainlog;
    delete log;
    delete ui;
}

void mainForm::recieveInput()
{
    digitalWrite(OGATE1,0);
    digitalWrite(OGATE2,0);
    tGateTimer.stop();
    this->drawdisp(0,sInitTxt);
}

void mainForm::keyPressEvent( QKeyEvent *event )
{


    long serial=0;
    QString test;
    QString testchar;
    qDebug() << event->text() ;
    if (scanner == "")
        tTi.restart();
    if(event->key() == Qt::Key_Shift)
    {
        event->ignore();
        return;
    }
    if(event->text() == "\r"){
        serial++;
        getserverresp(scanner);
        iTi = tTi.elapsed();
        scanner = "";

    }
    else{
//        ui->label->setText("");
        scanner +=event->text();
    }

}

void mainForm::getserverresp(QString inp)
{
    qDebug() <<  inp;

    if (tGateTimer.remainingTime() > 0)
    {
        this->drawdisp(0,sWaitTxt);
        return ;

    }

    QUrl serviceUrl = QUrl(sBaseUrl);
    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;

    qDebug() << sBaseUrl;

    json.insert("GateType", iGateType);
    json.insert("QrData", inp);
    json.insert("Password", sPassword);
    json.insert("Username", sUserName);
    json.insert("DeviceId", iDeviceID);

    QJsonDocument doc(json);
    QByteArray ba = doc.toJson();
    qDebug() << json;
    mainlog->GetCurrentStream() << QString(ba) << endl;
    tWe.restart();
    qDebug() << myNetworkAccessManager->post(request,QJsonDocument(json).toJson());

}

void mainForm::replyFinished(QNetworkReply *reply)
{
    qDebug() << "finished!";
    QByteArray response_data = reply->readAll();
    QJsonDocument json_doc;
    QJsonParseError error_json;
    QJsonDocument jsonresp = QJsonDocument::fromJson(response_data, &error_json);
    QJsonObject jo = jsonresp.object();
    iWe = tWe.elapsed();
    tRe.restart();


    mainlog->GetCurrentStream() << jsonresp.toJson() << endl;
    if (jsonresp.isNull()){
        iSST = 0 ;
        qDebug() << "is null: " << error_json.errorString();
        iRe = tRe.elapsed();
        drawdisp(0,  sInitTxt );
    }
    else{
        iSST = 1 ;
        tGateTimer.stop();
        qDebug() << "is not null";
        if (jo.value(QString("Status")).toBool(0) ){
            digitalWrite(OGATE1,1);
            digitalWrite(OGATE2,1);
            iRe = tRe.elapsed();
            mainlog->GetCurrentStream() << "relayout" << endl;
            drawdisp(1,  jo.value(QString("Message")).toString("خظا"));
            tGateTimer.start(iRelayTimeout);

        }
        else{
            iRe = tRe.elapsed();
            drawdisp(2,  jo.value(QString("Message")).toString("خطا"));
            tGateTimer.start(1000);
        }
    }

    reply->deleteLater();
    qDebug() << jsonresp.toJson();

}

QString mainForm::getcurrentIP()
{
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        iNST = 1;
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             return  address.toString().replace(".","A");
    }
    iNST = 0;
    return "";
}

void mainForm::drawdisp(int iSt,QString sMsg )
{

    QPalette pal = palette();
    QColor cTemp;
    if (iSt == 0 ){
        pal.setColor(QPalette::Background, Qt::white);
        cTemp = QColor(cNColor);
        lblMainImg->setPixmap(this->pnBG);
    }
    else if (iSt == 1){
        cTemp = QColor(cOColor);
        pal.setColor(QPalette::Background, cTemp);
        lblMainImg->setPixmap(this->psBG);
    }
    else if (iSt == 2){
        cTemp = QColor(cFColor);
        pal.setColor(QPalette::Background, cTemp);
        lblMainImg->setPixmap(this->pfBG);
    }

    this->setAutoFillBackground(true);
    this->setPalette(pal);

    lblLogoImg->setPixmap(pILogo);

    if (iNST > 0 )
        lblNetImg->setPixmap(pINO);
    else
        lblNetImg->setPixmap(pINF);

    if (iSST > 0 )
        lblSrvImg->setPixmap(pISO);
    else
        lblSrvImg->setPixmap(pISF);

    if (iSST == 0 || iNST == 0 )
        lblAllImg->setPixmap(pIOF);
    else
        lblAllImg->setPixmap(pIOO);

    lblIP->setText(getcurrentIP());

    lblTimes->setText( QString("ti:%1 we:%2 re:%3").arg(QString::number(iTi), QString::number(iWe), QString::number(iRe)) );
    lblMsg->setText(sMsg);
    lblMsg->setStyleSheet("QLabel {  color : "+cTemp.name()+"; }");
}
