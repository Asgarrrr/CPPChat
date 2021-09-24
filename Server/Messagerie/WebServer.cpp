#include "WebServer.h"
#include "Server.h"

WebServer::WebServer(Database * db, QObject *parent): QObject(parent) {
	webServer = new QWebSocketServer(QStringLiteral("WebServer"), QWebSocketServer::NonSecureMode, this);
	QObject::connect(webServer, &QWebSocketServer::newConnection, this, &WebServer::onWebServerNewConnection);
	
	this->db = db;
	webServer->listen(QHostAddress::AnyIPv4, 4456);
	
}

WebServer::~WebServer()
{
}
//permet d'inclure la classe server dans webServer
void WebServer::setServer(Server * server)
{
	this->server = server;

	server->getAllTcpClientsConnection();
}
//retourne le tableau des clients web connecté
QVector<QWebSocket *> WebServer::getAllWebClientsConnection()
{
	return allWebClients;
}

void WebServer::onWebServerNewConnection()
{
	QWebSocket * webClient = webServer->nextPendingConnection();
	QTcpSocket::connect(webClient, &QWebSocket::textMessageReceived, this,&WebServer::onWebClientCommunication);
	QTcpSocket::connect(webClient, &QWebSocket::disconnected, this, &WebServer::onWebClientDisconnected);
	allWebClients.append(webClient);
}

void WebServer::onWebClientDisconnected()
{
	QWebSocket * obj = qobject_cast<QWebSocket*>(sender());
	QObject::disconnect(obj, &QWebSocket::textMessageReceived, this, &WebServer::onWebClientCommunication);
	QObject::disconnect(obj, &QWebSocket::disconnected, this, &WebServer::onWebClientDisconnected);
	allWebClients.removeOne(obj);
	obj->deleteLater();
}

void WebServer::onWebClientCommunication()
{
}


