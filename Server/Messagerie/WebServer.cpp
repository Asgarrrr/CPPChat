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
//retourne le tableau des clients web connect�
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

void WebServer::onWebClientCommunication(QString entryMessage)
{
	//�coute le client puis r�cup�re son message
	QWebSocket * obj = qobject_cast<QWebSocket*>(sender());
	QString data = entryMessage;

	QString cryptLogin, cryptPass, login, pass, message, inscriptionLogin, inscriptionPass, inscriptionPseudo;
	int ID;
	//Expression reguli�re pour le type de message envoy�
	QRegExp regExp("^code:(\\d{0,2})");
	int codePos = regExp.indexIn(data);
	QStringList list = regExp.capturedTexts();

	//Expression reguli�re pour l'authentification
	QRegExp regExpLogin("login:(.+)password:(.+)$");
	int codePosLogin = regExpLogin.indexIn(data);
	QStringList listLogin = regExpLogin.capturedTexts();

	//Expression reguli�re pour l'inscription
	QRegExp regExpRegister("login:(.+)password:(.+)pseudo:(.+)$");
	int codePosRegister = regExpRegister.indexIn(data);
	QStringList listRegister = regExpRegister.capturedTexts();

	//Expression reguli�re pour le message
	QRegExp regExpMessage("ID:(\\d+)message:(.+)$");
	int codePosMessage = regExpMessage.indexIn(data);
	QStringList listMessage = regExpMessage.capturedTexts();

	//effectue la connexion, l'inscription ou gere les messages
	switch (list.at(1).toInt())
	{
		//cas ou code = 1 est une authentification
	case 1:
	{
		//recuperation des infos de connexion
		login = listLogin.at(1).toUtf8();
		pass = listLogin.at(2).toUtf8();
		qDebug() << login << pass;
		ID = db->login(login, pass);

		//Envoie la r�ponse au client avec l'ID
		QString response = "code:01ID:" + QString::number(ID);
		obj->sendTextMessage(response);

		//recup�re les 100 derni�res messages et les stocke dans un tableau
		std::vector<std::string> lastMessages;
		lastMessages = db->sendLastMessagesToClient();
		//envoie le message a tout les utilisateurs du tableau
		for (std::string message : lastMessages) {
			obj->sendTextMessage(message.c_str());
		}



	}
	break;
	//cas ou code = 2 est l'inscription
	case 2:
	{
		//recuperation des infos d'inscription
		inscriptionLogin = listRegister.at(2).toUtf8();
		inscriptionPass = listRegister.at(3).toUtf8();
		inscriptionPseudo = listRegister.at(4).toUtf8();
		ID = db->inscription(inscriptionLogin, inscriptionPass);//, inscriptionPseudo
		//a changer pour l'inscription
		//QString response = "code:01ID:" + QString::number(ID);
		//obj->sendTextMessage(response);
	}
	break;
	//cas ou code = 3 est un message
	case 3:
	{

		qDebug() << listMessage;
		ID = listMessage.at(1).toInt();
		message = listMessage.at(2).toUtf8();
		std::string pseudo = db->sendMessageInDB(ID, message);
		//recup�re le tableau des connexions web
		QVector<QTcpSocket *> allTcpClients;
		allTcpClients = server->getAllTcpClientsConnection();
		QString sendMessage = "code:03ID:" + QString::number(ID) + "pseudo:" + pseudo.c_str() + "message:" + message.toUtf8();
		//envoie message TCP
		for (QTcpSocket *socket : allTcpClients) {
			socket->write(sendMessage.toUtf8());//envoyer le message ici
		}
		//envoie message web
		for (QWebSocket *webSocket : allWebClients) {
			webSocket->sendTextMessage(sendMessage.toUtf8());//envoyer le message ici
		}
	}
	break;

	default:
		break;
	}


	qDebug() << data;

}

