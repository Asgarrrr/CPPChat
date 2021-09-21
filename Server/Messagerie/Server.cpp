#include "Server.h"

Server::Server( QObject *parent ) : QObject( parent ) {
	server = new QTcpServer(this);

	QObject::connect(server, SIGNAL(newConnection()), this, SLOT(onServerNewConnection()));

	server->listen(QHostAddress::AnyIPv4, 4444);
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
	allClients.append(client);


	
	qDebug() << "suck";
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
	//repasse en tableau en acceptant les caracteres speciaux
	QByteArray arrayData = strData.toUtf8();
	//transforme le tableau en docuement
	QJsonDocument jsonData = QJsonDocument::fromJson(arrayData);
	//et le transforme en objet lisible
	QJsonObject objectData = jsonData.object();

	QString cryptLogin, cryptPass, login, pass, message, inscriptionLogin, inscriptionPass, inscriptionPseudo;
	//client
	QDataStream returnObj(obj);
	QJsonObject returnData;

	qDebug() << objectData;
	
	//effectue la connexion, l'inscription ou gere les messages
	switch (objectData["code"].toInt())
	{
		//cas ou code = 1 est une authentification
	case 1:
		//recuperation des infos de connexion
		login = objectData["login"].toString();
		pass = objectData["password"].toString();
		//test bdd si existant true sinon false et creation du renvoie en fonction du cas
		if (db->login(login, pass) == true) {
			returnData["code"] = 1;
			returnData["value"] = 1;
			qDebug() << "true";
		}
		else {
			returnData["code"] = 1;
			returnData["value"] = 0;
			qDebug() << "false";
		}
		//renvoie l'objet json au client
		returnObj << returnData;
		qDebug() << login << pass;

		break;
		//cas ou code = 2 est un message
	case 2:
		message = objectData["message"].toString();
		db->sendMessageInDB(login, pass, message);
		for (QTcpSocket *allClients : allClients) {
			returnObj << returnData;//envoyer le message ici
		}
		break;
		//cas ou code = 3 est l'inscription
	case 3:
		//recuperation des infos d'inscription
		inscriptionLogin = objectData["inscriptionLogin"].toString();
		inscriptionPass = objectData["inscriptionLogin"].toString();
		inscriptionPseudo = objectData["inscriptionLogin"].toString();
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
	
	qDebug() << data;
	
}



void Server::onClientConnection()
{
}
