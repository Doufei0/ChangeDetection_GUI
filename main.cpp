#include "ChangeDetectionApplication.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChangeDetectionApplication w;
    w.show();
    return a.exec();
}
