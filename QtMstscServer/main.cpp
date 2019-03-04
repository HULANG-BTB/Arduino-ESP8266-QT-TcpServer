#include "mstscserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MstscServer w;
    w.show();

    return a.exec();
}
