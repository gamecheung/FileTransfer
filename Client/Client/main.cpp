#include "clienttransform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientTransform w;
    w.show();

    return a.exec();
}
