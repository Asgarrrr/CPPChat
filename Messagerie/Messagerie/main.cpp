#include "Messagerie.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Messagerie w;
    w.show();
    return a.exec();
}
