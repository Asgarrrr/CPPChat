#include "Server.h"
#include "WebServer.h"
#include "database.h"
#include <QtCore/QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	Database db = new Database;
	db.connectToDB();
	Server server(&db);
	WebServer webServer(&db);
	server.setWebServer(&webServer);
	webServer.setServer(&server);
    return a.exec();
}
