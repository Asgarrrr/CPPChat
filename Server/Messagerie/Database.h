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

	void connectToDB();
	int login(QString login, QString pass);
	QString inscription(QString inscriptionLogin, QString inscriptionPass, QString inscriptionPseudo);
	std::vector<std::string> sendLastMessagesToClient();
	std::string sendMessageInDB(int ID, QString message);
};


