#pragma once

#include "database.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <QVector>

class WebServer;

class Server : public QObject
{
	Q_OBJECT

private:
	QTcpServer * server;
	Database * db;
	QVector<QTcpSocket *> allTcpClients;
	WebServer * webServer;

public:
	Server(Database * db, QObject *parent = Q_NULLPTR);
	~Server();

	//permet d'inclure la classe webServer dans la classe server
	void setWebServer(WebServer * webServer);
	QVector<QTcpSocket *> getAllTcpClientsConnection();

public slots:
	void onServerNewConnection();
	void onClientCommunication();
	void onClientDisconnected();
	
};
