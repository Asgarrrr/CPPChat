#pragma once

#include <QObject>
#include <qwebsocket.h>
#include <QWebSocketServer>
#include "Database.h"

class Server;

class WebServer : public QObject
{
	Q_OBJECT

public:
	WebServer(Database * db, QObject *parent = Q_NULLPTR);
	~WebServer();
	//permet d'inclure la classe server dans webServer
	void setServer(Server * server);
	QVector<QWebSocket *> getAllWebClientsConnection();
private:
	QWebSocketServer * webServer;
	Database * db;
	Server * server;
	QVector<QWebSocket *> allWebClients;

public slots:
	void onWebServerNewConnection();
	void onWebClientDisconnected();
	void onWebClientCommunication(QString entryMessage);
};
