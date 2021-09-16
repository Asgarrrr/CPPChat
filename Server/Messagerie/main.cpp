#include "Messagerie.h"
#include "Server.h"
#include "Database.h"
#include <QtWidgets/QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	Server server;
	Database database;
	database.connectToDB();

    Messagerie w;
    w.show();
    return a.exec();
}
