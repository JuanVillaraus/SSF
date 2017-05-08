#include "siviso.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    siviso w;

    w.m_pApplication = &a;
            w.changeStyleSheet(DAY_STYLE);

    w.show();
    //w.setGeometry(1300,1300,1,1);
    w.setGeometry(0,0,1,1);
    w.showMaximized();

    return a.exec();
}
