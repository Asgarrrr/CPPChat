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
		qDebug() << "Connexion a la BDD reussie";
	}
	else {
		qDebug() << "Erreur de connexion a la BDD";
		exit(1);
	}
}
QString Database::inscription(QString inscriptionLogin, QString inscriptionPass, QString inscriptionPseudo)
{
	QSqlQuery insert;
	QSqlQuery request;
	qDebug() << inscriptionLogin << inscriptionPass;

	if ((Database::login(inscriptionLogin, inscriptionPass)) == 0) {

		insert.prepare("INSERT INTO `users`(`_login`, `_password`, `_pseudo`) VALUES (?,?,?)");
		insert.addBindValue(inscriptionLogin);
		insert.addBindValue(inscriptionPass);
		insert.addBindValue(inscriptionPseudo);
		if (insert.exec()) {
			request.prepare("SELECT `_ID`, `_pseudo` FROM `users` WHERE _login = ? AND _password = ? limit 1");
			request.addBindValue(inscriptionLogin);
			request.addBindValue(inscriptionPass);
			request.exec();
			request.first();
			qDebug() << "Un utilisateur s'est inscrit";
			return "code:01ID:" + request.value(0).toString() + "pseudo:" + request.value(1).toString();
		}
		else {
			return "code:01ID:0";
		}
	}
	else {

		return "code01:ID:0";
	}
}
std::vector<std::string> Database::sendLastMessagesToClient()
{
	std::vector<std::string> result;
	QString pseudo, message;

	QSqlQuery request;
	request.prepare("SELECT `_pseudo`,`_message` FROM `message` WHERE 1 ORDER BY `_ID` DESC LIMIT 100");
	request.exec();
	request.last();

	do {
		pseudo = request.value(0).toString();
		message = request.value(1).toString();
		result.push_back("code:03pseudo:" + pseudo.toStdString() + "message:" + message.toStdString());
	} while (request.previous());
	return result;
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
//enregistre le message dans la BDD et retourne le pseudo de l'emetteur du message
std::string Database::sendMessageInDB(int ID, QString message)
{
	QString pseudo;
	QVariant pseudoTemp;
	QSqlQuery requestPseudo;
	requestPseudo.prepare("SELECT `_pseudo` FROM `users` WHERE `_ID` = ?");

	QSqlQuery requestInsertMessage;
	requestPseudo.addBindValue(ID);
	requestPseudo.exec();
	requestPseudo.first();
	pseudoTemp = requestPseudo.value(0);
	pseudo = pseudoTemp.toString();

	requestInsertMessage.prepare("INSERT INTO `message`(`_pseudo`, `_message`) VALUES (?,?)");
	requestInsertMessage.addBindValue(pseudo);
	requestInsertMessage.addBindValue(message);
	requestInsertMessage.exec();
	qDebug() << "Le message a ete stocke dans la BDD";
	return pseudo.toStdString();
}