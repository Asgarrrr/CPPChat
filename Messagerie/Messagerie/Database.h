#pragma once

#include <QObject>

class Database : public QObject
{
	Q_OBJECT

public:
	Database( QObject *parent = Q_NULLPTR );
	~Database();
};
