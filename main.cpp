#include <QApplication>
#include <QThread>
#include <QSplashScreen>
#include "sleeper.h"
#include "style.h"
#include "initwindow.h"
#include "genetixwindow.h"

//class T : public QThread
//{
//    void run() { Sleeper::sleep(5); }
//};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(Style::logo());
    QSplashScreen splash(pixmap);
    splash.setEnabled(false);
    splash.show();
    Sleeper::msleep(20);
    a.processEvents();

    //display splash screen
    for(int i = 0; i <= 4; i++) {
        a.processEvents();
        Sleeper::msleep(500);
    }

    initwindow init;
    GenetixWindow Gw;
    splash.finish(&init);
    init.show();
    Gw.show();

    a.exec();
}

