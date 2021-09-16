#pragma once

#include <QObject>
#include <qsqldatabase.h>

class Database : public QObject
{
	Q_OBJECT

private:
	QSqlDatabase * db;

public:
	Database(QObject *parent = Q_NULLPTR);
	~Database();

	void sendMessageInDB();
	void closeDB();
	void connectToDB();
};


