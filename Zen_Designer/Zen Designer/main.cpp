#include "ZenDesigner.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZenDesigner w;
    w.show();
    return a.exec();
}
