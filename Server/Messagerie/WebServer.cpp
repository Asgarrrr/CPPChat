#include "WebServer.h"
#include "Server.h"

WebServer::WebServer(Database * db, QObject *parent): QObject(parent) {
	webServer = (new QWebSocketServer(QStringLiteral("WebServer"), QWebSocketServer::NonSecureMode, this));
	QObject::connect(webServer, &QWebSocketServer::newConnection, this, &WebServer::onWebServerNewConnection);
	
	this->db = db;
	webServer->listen(QHostAddress::AnyIPv4, 4457);
	
}

WebServer::~WebServer()
{
}

//permet d'inclure la classe server dans webServer
void WebServer::setServer(Server * server)
{
	this->server = server;

}

//retourne le tableau des clients web connecté
QVector<QWebSocket *> WebServer::getAllWebClientsConnection()
{
	return allWebClients;
}

//enregistre la connexion du client et l'ajoute à un tableau de connexion
void WebServer::onWebServerNewConnection()
{
	QWebSocket * webClient = webServer->nextPendingConnection();
	QTcpSocket::connect(webClient, &QWebSocket::textMessageReceived, this,&WebServer::onWebClientCommunication);
	QTcpSocket::connect(webClient, &QWebSocket::disconnected, this, &WebServer::onWebClientDisconnected);
	allWebClients.append(webClient);

	qDebug() << "Un client WEB s'est connecte";
}

void WebServer::onWebClientCommunication(QString entryMessage)
{
	//écoute le client puis récupère son message
	QWebSocket * obj = qobject_cast<QWebSocket*>(sender());
	QString data = entryMessage;

	QString cryptLogin, cryptPass, login, pass, message, inscriptionLogin, inscriptionPass, inscriptionPseudo;
	int ID;
	//Expression regulière pour le type de message envoyé
	QRegExp regExp("^code:(\\d{0,2})");
	int codePos = regExp.indexIn(data);
	QStringList list = regExp.capturedTexts();

	//Expression regulière pour l'authentification
	QRegExp regExpLogin("login:(.+)password:(.+)$");
	int codePosLogin = regExpLogin.indexIn(data);
	QStringList listLogin = regExpLogin.capturedTexts();

	//Expression regulière pour l'inscription
	QRegExp regExpRegister("login:(.+)password:(.+)pseudo:(.+)$");
	int codePosRegister = regExpRegister.indexIn(data);
	QStringList listRegister = regExpRegister.capturedTexts();

	//Expression regulière pour le message
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
		std::string response = db->login(login, pass);
		//
		if (response != "0") {
			//Envoie la réponse au client avec l'ID
			obj->sendTextMessage(response.c_str());

			/*//Envoie le fait qu'un utilisateur soit connecté à tout les utilisateurs
			QVector<QTcpSocket *> allTcpClients;
			allTcpClients = server->getAllTcpClientsConnection();
			//envoie message TCP
			for (QTcpSocket *socket : allTcpClients) {
				socket->write(response.c_str());//envoyer le message ici
			}
			qDebug() << "La connexion WEB a ete envoye a tout les clients TCP";
			//envoie message web
			for (QWebSocket *webSocket : allWebClients) {
				webSocket->sendTextMessage(response.c_str());//envoyer le message ici
			}
			qDebug() << "La connexion WEB a ete envoye a tout les clients WEB";*/
		}
		else {
			obj->sendTextMessage("code:01ID:0");
		}

		if (response == "0")
			return;
		//recupère les 100 dernières messages et les stocke dans un tableau
		std::vector<std::string> lastMessages;
		lastMessages = db->sendLastMessagesToClient();
		//envoie le message a tout les utilisateurs du tableau
		for (std::string message : lastMessages) {
			obj->sendTextMessage(message.c_str());
		}

		qDebug() << "Un compte s'est connecte";

	}
	break;
	//cas ou code = 2 est l'inscription
	case 2:
	{
		//recuperation des infos d'inscription
		inscriptionLogin = listRegister.at(1).toUtf8();
		inscriptionPass = listRegister.at(2).toUtf8();
		inscriptionPseudo = listRegister.at(3).toUtf8();
		QString response = db->inscription(inscriptionLogin, inscriptionPass, inscriptionPseudo);
		obj->sendTextMessage(response);

		//recupère les 100 dernières messages et les stocke dans un tableau
		std::vector<std::string> lastMessages;
		lastMessages = db->sendLastMessagesToClient();
		//envoie le message a tout les utilisateurs du tableau
		for (std::string message : lastMessages) {
			obj->sendTextMessage(message.c_str());
		}

		qDebug() << "Un compte s'est deconnecte";
	}
	break;
	//cas ou code = 3 est un message
	case 3:
	{

		qDebug() << listMessage;
		ID = listMessage.at(1).toInt();
		message = listMessage.at(2).toUtf8();
		std::string pseudo = db->sendMessageInDB(ID, message);
		//recupère le tableau des connexions web
		QVector<QTcpSocket *> allTcpClients;
		allTcpClients = server->getAllTcpClientsConnection();
		QString sendMessage = "code:03ID:" + QString::number(ID) + "pseudo:" + pseudo.c_str() + "message:" + message.toUtf8();
		//envoie message TCP
		for (QTcpSocket *socket : allTcpClients) {
			socket->write(sendMessage.toUtf8());//envoyer le message ici
		}
		qDebug() << "Le message WEB a ete envoye a tout les clients TCP";
		//envoie message web
		for (QWebSocket *webSocket : allWebClients) {
			webSocket->sendTextMessage(sendMessage.toUtf8());//envoyer le message ici
		}
		qDebug() << "Le message WEB a ete envoye a tout les clients WEB";
	}
	break;

	default:
		break;
	}
}

//Déconnecte le client et le supprime du tableau de connexion
void WebServer::onWebClientDisconnected()
{
	QWebSocket * obj = qobject_cast<QWebSocket*>(sender());
	QObject::disconnect(obj, &QWebSocket::textMessageReceived, this, &WebServer::onWebClientCommunication);
	QObject::disconnect(obj, &QWebSocket::disconnected, this, &WebServer::onWebClientDisconnected);
	allWebClients.removeOne(obj);
	obj->deleteLater();

	qDebug() << "Un client WEB s'est deconnecte";
}
