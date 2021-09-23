#include "Server.h"
#include <QtCore/QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	Server server;

    return a.exec();
}
