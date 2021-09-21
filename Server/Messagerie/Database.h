#pragma once

#include <QObject>
#include <qsqldatabase.h>
#include <QDebug>
#include <QtSql/QtSql>

class Database : public QObject
{
	Q_OBJECT

private:
	QSqlDatabase * db;

public:
	Database(QObject *parent = Q_NULLPTR);
	~Database();

	void sendMessageInDB(QString login, QString pass, QString message);
	bool login(QString login, QString pass);
	void connectToDB();
	char inscription(QString inscriptionLogin, QString inscriptionPass, QString inscriptionPseudo);
};


