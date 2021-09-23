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
		qDebug() << "Connect�";
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
int Database::login(QString login, QString pass)
{
	QSqlQuery request;
	request.prepare("SELECT * FROM `users` WHERE _login = ? AND _password = ? limit 1");
	request.addBindValue(login);
	request.addBindValue(pass);

	request.exec();

	request.first();

	return request.size() == 1 ? request.value( 0 ).toInt() : 0;

}
//enregistre le message dans la BDD
std::string Database::sendMessageInDB(int ID, QString message)
{
	QString pseudo;
	QVariant pseudoTemp;
	QSqlQuery requestPseudo;
	requestPseudo.prepare("SELECT `_pseudo` FROM `users` WHERE `_ID` = ?");

	QSqlQuery requestInsertMessage;
	requestPseudo.addBindValue(ID);
	requestPseudo.exec();
	pseudoTemp = requestPseudo.value(0);
	pseudo = pseudoTemp.toString();

	requestInsertMessage.prepare("INSERT INTO `message`(`_pseudo`, `_message`) VALUES (?,?)");
	requestInsertMessage.addBindValue(pseudo);
	requestInsertMessage.addBindValue(message);
	requestInsertMessage.exec();

	return pseudo.toStdString();
}