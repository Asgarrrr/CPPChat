#pragma once

#include "database.h"

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qwebsocket.h>
#include <QVector>

class Server : public QObject
{
	Q_OBJECT

public:
	Server(QObject *parent = Q_NULLPTR);
	~Server();
private:
	QTcpServer * server;
	QWebSocket * WebSocket;
	Database * db;
	QVector<QTcpSocket *> allClients;

public slots:
	void onServerNewConnection();
	void onClientDisconnected();
	void onClientCommunication();
	void onClientConnection();
};
