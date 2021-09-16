#include "Database.h"

Database::Database(QObject *parent)
	: QObject(parent)
{
	
}

Database::~Database()
{
}

void Database::sendMessageInDB()
{

}

void Database::closeDB()
{
	db->close();
}

void Database::connectToDB()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.65.27");
	db.setDatabaseName("wishcord");
	db.setUserName("root");
	db.setPassword("root");
	bool ok = db.open();
	if (db.open())
	{
		qDebug
	}
}
