#include "servertransform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerTransform w;
    w.show();

    return a.exec();
}
