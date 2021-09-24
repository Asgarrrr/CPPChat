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

public:
	Server(Database * db, QObject *parent = Q_NULLPTR);
	~Server();
	QVector<QTcpSocket *> & getAllTcpClientsConnection();
	//permet d'inclure la classe webServer dans la classe server
	void setWebServer(WebServer * webServer);


private:
	QTcpServer * server;
	Database * db;
	QVector<QTcpSocket *> allTcpClients;
	WebServer * webServer;

public slots:
	void onServerNewConnection();
	void onClientDisconnected();
	void onClientCommunication();
	
};
