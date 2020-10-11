#include "magnetometer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Magnetometer w;
    w.show();
    return a.exec();
}
