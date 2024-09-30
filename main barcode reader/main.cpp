#include "mainform.h"
#include <QApplication>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    mainForm frm;
//    MainWindow w;
//    w.show();
    frm.setWindowTitle("MiladTower");
    frm.setWindowFlags(Qt::FramelessWindowHint);
    frm.show();

    return a.exec();
}
