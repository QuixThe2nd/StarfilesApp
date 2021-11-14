#include "starfiles.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Starfiles w;
    a.setStyle("fusion");
    w.show();
    return a.exec();
}
