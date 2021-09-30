#include "Server.h"
#include "WebServer.h"
Server::Server( Database * db, QObject *parent ) : QObject( parent ) {
	server = new QTcpServer(this);
	QObject::connect(server, SIGNAL(newConnection()), this, SLOT(onServerNewConnection()));
	
	this->db = db;
	server->listen(QHostAddress::AnyIPv4, 4456);
}

Server::~Server()
{
}

//retourne le tableau des clients tcp connecté
QVector<QTcpSocket*> Server::getAllTcpClientsConnection()
{
	return allTcpClients;
}

//permet d'inclure la classe webServer dans server
void Server::setWebServer(WebServer * webServer)
{
	this->webServer = webServer;
}

//enregistre la connexion du client et l'ajoute à un tableau de connexion
void Server::onServerNewConnection()
{
	
	QTcpSocket * client = server->nextPendingConnection();
	QTcpSocket::connect(client, SIGNAL(readyRead()), this, SLOT(onClientCommunication()));
	QTcpSocket::connect(client, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
	allTcpClients.append(client);


	
	qDebug() << "Un client TCP s'est connecte";
}

// communication client .
void Server::onClientCommunication()
{	//écoute le client puis récupère son message
	QTcpSocket * obj = qobject_cast<QTcpSocket*>(sender());
	QByteArray data = obj->read(obj->bytesAvailable());
	
	//passe data de tableau a string
	QString strData( data );

	QString cryptLogin, cryptPass, login, pass, message, inscriptionLogin, inscriptionPass, inscriptionPseudo;
	int ID;
	//Expression regulière pour le type de message envoyé
	QRegExp regExp("^code:(\\d{0,2})");
	int codePos = regExp.indexIn(strData);
	QStringList list = regExp.capturedTexts();

	//Expression regulière pour l'authentification
	QRegExp regExpLogin("login:(.+)password:(.+)$");
	int codePosLogin = regExpLogin.indexIn(strData);
	QStringList listLogin = regExpLogin.capturedTexts();

	//Expression regulière pour l'inscription
	QRegExp regExpRegister("login:(.+)password:(.+)pseudo:(.+)$");
	int codePosRegister = regExpRegister.indexIn(strData);
	QStringList listRegister = regExpRegister.capturedTexts();

	//Expression regulière pour le message
	QRegExp regExpMessage("ID:(\\d+)message:(.+)$");
	int codePosMessage = regExpMessage.indexIn(strData);
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
			ID = db->login(login, pass);

			//Envoie la réponse au client avec l'ID
			QString response = "code:01ID:" + QString::number(ID);
			obj->write(response.toStdString().c_str());
			

			//Envoie les 100 dernières messages au client
			std::vector<std::string> lastMessages;
			lastMessages = db->sendLastMessagesToClient();

			for (std::string message : lastMessages) {
				obj->write(message.c_str());//envoyer le message ici
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
			obj->write(response.toStdString().c_str());

			//Envoie les 100 dernières messages au client
			std::vector<std::string> lastMessages;
			lastMessages = db->sendLastMessagesToClient();

			for (std::string message : lastMessages) {
				obj->write(message.c_str());//envoyer le message ici
			}

			qDebug() << "Un compte s'est connecte";
		}
		break;
		//cas ou code = 3 est un message
	case 3:
		{

		qDebug() << "Un message a ete recu";
			ID = listMessage.at(1).toInt();
			message = listMessage.at(2).toUtf8();
			std::string pseudo = db->sendMessageInDB(ID, message);
			//recupère le tableau des connexions web
			QVector<QWebSocket *> allWebClients;
			allWebClients = webServer->getAllWebClientsConnection();
			QString sendMessage = "code:03ID:" + QString::number(ID) + "pseudo:" + pseudo.c_str() + "message:" + message.toUtf8();
			//envoie message TCP
			for (QTcpSocket *socket : allTcpClients) {
				socket->write(sendMessage.toUtf8());//envoyer le message ici
			}
			qDebug() << "Le message TCP a ete envoye a tout les clients TCP";
			//envoie message web
			for (QWebSocket *webSocket : allWebClients) {
				webSocket->sendTextMessage(sendMessage.toUtf8());//envoyer le message ici
			}
			qDebug() << "Le message TCP a ete envoye a tout les clients WEB";
		}
		break;

	default:
		break;
	}
}

//Déconnecte le client et le supprime du tableau de connexion
void Server::onClientDisconnected()
{
	QTcpSocket * obj = qobject_cast<QTcpSocket*>(sender());
	QObject::disconnect(obj, SIGNAL(readyRead()), this, SLOT(onClientCommunication()));
	QObject::disconnect(obj, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
	allTcpClients.removeOne(obj);
	obj->deleteLater();

	qDebug() << "Un client TCP s'est deconnecte";
}



