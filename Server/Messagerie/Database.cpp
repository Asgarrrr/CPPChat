#include "Database.h"

Database::Database(QObject *parent)
	: QObject(parent)
{
	
}

Database::~Database()
{
}

// connexion BDD 
void Database::connectToDB()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.65.27");
	db.setDatabaseName("wishcord");
	db.setUserName("root");
	db.setPassword("root");

	if (db.open())
	{
		qDebug() << "Connecté";
	}
	else {
		qDebug() << "Erreur de connexion";
		exit(1);
	}
}
char Database::inscription(QString inscriptionLogin, QString inscriptionPass, QString inscriptionPseudo)
{
	QSqlQuery request;

	request.prepare("INSERT INTO `users`(`_login`, `_password`, `_pseudo`) VALUES (?,?,?)");
	request.addBindValue(inscriptionLogin);
	request.addBindValue(inscriptionPass);
	request.addBindValue(inscriptionPseudo);
	switch (request.exec())
	{
	case true:
		return 't';
		break;
	case false:
		return 'f';
		break;
	default:
		return 'e';
		break;
	}
}
//Verifie la connexion
bool Database::login(QString login, QString pass)
{
	QSqlQuery request;

	request.prepare("SELECT * FROM `users` WHERE _login = ? AND _password = ? ");
	request.addBindValue(login);
	request.addBindValue(pass);
	request.exec();

	qDebug() << request.value(0);

	

	if (request.isNull('_ID')) {
		qDebug() << "false";
		return false;
	}
	else {
		qDebug() << "true";
		return true;
	}

}
//enregistre le message dans la BDD
void Database::sendMessageInDB(QString login, QString pass, QString message)
{
	QString pseudo;
	QVariant pseudoTemp;
	QSqlQuery requestPseudo;
	requestPseudo.prepare("SELECT `_pseudo` FROM `users` WHERE `_login` = ? AND `_password` = ?");

	QSqlQuery requestInsertMessage;
	requestPseudo.addBindValue(login);
	requestPseudo.addBindValue(pass);
	requestPseudo.exec();
	pseudoTemp = requestPseudo.value(0);
	pseudo = pseudoTemp.toString();

	requestInsertMessage.prepare("INSERT INTO `message`(`_pseudo`, `_message`) VALUES (?,?)");
	requestInsertMessage.addBindValue(pseudo);
	requestInsertMessage.addBindValue(message);
	requestInsertMessage.exec();
}