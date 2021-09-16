#pragma once

#include "database.h";

#include <QObject>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qwebsocket.h>

class Server : public QObject
{
	Q_OBJECT

public:
	Server(QObject *parent = Q_NULLPTR);
	~Server();
private:
	QTcpServer * server;
	Database * database;
	QWebSocket * WebSocket;

public slots:
	void onServerNewConnection();
	void onClientDisconnected();
	void onClientReadyRead();
	void onClientConnection();
};
