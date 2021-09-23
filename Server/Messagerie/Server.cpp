#include "Server.h"

Server::Server( QObject *parent ) : QObject( parent ) {
	server = new QTcpServer(this);

	QObject::connect(server, SIGNAL(newConnection()), this, SLOT(onServerNewConnection()));

	server->listen(QHostAddress::AnyIPv4, 4456);
	db = new Database(this);
	db->connectToDB();

}



Server::~Server()
{
}

void Server::onServerNewConnection()
{
	QTcpSocket * client = server->nextPendingConnection();
	QTcpSocket::connect(client, SIGNAL(readyRead()), this, SLOT(onClientCommunication()));
	QTcpSocket::connect(client, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
	allTcpClients.append(client);


	
	qDebug() << "I te suck";
}



void Server::onClientDisconnected()
{
	QTcpSocket * obj = qobject_cast<QTcpSocket*>(sender());
	QObject::disconnect(obj, SIGNAL(readyRead()), this, SLOT(onClientCommunication()));
	QObject::disconnect(obj, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
	obj->deleteLater();
}
// communication client .
void Server::onClientCommunication()
{	//écoute le client puis récupère son message
	QTcpSocket * obj = qobject_cast<QTcpSocket*>(sender());
	QByteArray data = obj->read(obj->bytesAvailable());
	
	//passe data de tableau a string
	QString strData( data );

	QString cryptLogin, cryptPass, login, pass, message, inscriptionLogin, inscriptionPass, inscriptionPseudo, pseudo;
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
	QRegExp regExpRegister("^code:(\\d{0,2})");
	int codePosRegister = regExpRegister.indexIn(strData);
	QStringList listRegister = regExpRegister.capturedTexts();

	//Expression regulière pour le message
	QRegExp regExpMessage("^code:(\\d{0,2})");
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
			qDebug() << login << pass;
			ID = db->login(login, pass);

			//Envoie la réponse au client avec l'ID
			QString test = "code:01ID:" + QString::number(ID);
			obj->write(test.toStdString().c_str());

		}
		break;
		//cas ou code = 2 est l'inscription
	case 2:
		{
			//recuperation des infos d'inscription
			inscriptionLogin = listRegister.at(2).toUtf8();
			inscriptionPass = listRegister.at(3).toUtf8();
			inscriptionPseudo = listRegister.at(4).toUtf8();
			//envoie BDD : Si réussi, cas 't', si echoue, cas 'f', si compte existant, cas 'e'
			switch (db->inscription(inscriptionLogin, inscriptionPass, inscriptionPseudo))
			{

			case 't':
				break;
			case 'f':
				break;
			case 'e':
				break;
			}
		}
		break;
		//cas ou code = 3 est un message
	case 3:
		{
			ID = listMessage.at(2).toInt;
			message = listMessage.at(3).toUtf8();
			std::string pseudo = db->sendMessageInDB(ID, message);
			//envoie message TCP
			//for (QTcpSocket *allClients : allTcpClients) {
			//	returnObj << returnData;//envoyer le message ici
			//}
			//envoie message web
			//for (QTcpSocket *allClients : allWebClients) {
				//returnObj << returnData;//envoyer le message ici
			//}
		}
		break;

	default:
		break;
	}
	
	qDebug() << data;
	
}




