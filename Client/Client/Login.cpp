#include "Login.h"
#include "Client.h"
#include "Register.h"
#include <qdebug.h>
#include <QCryptographicHash>

Login::Login( QWidget *parent ) : QWidget( parent ) {

	socket = new QTcpSocket(this);

	ui.setupUi(this);
	ui.labelLoginState->setText("Connecting ...");

	QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));

	socket->connectToHost( "192.168.65.103", 4456 );

	ui.lineEditPseudo->setVisible(false);
	ui.labelPseudo->setVisible(false);
	
}

void Login::onSocketConnected() {

	ui.labelLoginState->setText("Connected");

}

void Login::onSocketDisconnected() {
	qDebug() << "yesy";
}


void Login::onSocketReadyRead() {

	QByteArray res = socket->read(socket->bytesAvailable());
	QString str( res );

	qDebug() << str;

	QRegExp rx( "^code:(\\d{0,2})" );

	int codePos = rx.indexIn( str );

	QStringList list = rx.capturedTexts();

	switch ( list.at( 1 ).toInt() ) {
		
		// -- Login
		case 1: {

			QRegExp rx( "ID:(\\d+)" );
			int codePos = rx.indexIn( str );
			QStringList list = rx.capturedTexts();
	
			if ( list.at(1).toInt() != 0 ) {

				qDebug() << "Start ";
				
				ID = list.at( 1 ).toInt();

				c = new Client(socket, ID, NULL);
				ui.setupUi(this);
				c->show();
				logged = 1;
				this->close();

				// ———— Get last messages

				// —— Split masive block of message into individual message object
				QStringList lastMessagses = str.split( QString( "code:03" ) );

				// —— Loop over message
				for (QString message : lastMessagses) {

					QRegExp rx("pseudo:(.+)message:(.+)$");
					int codePos = rx.indexIn(message);
					QStringList list = rx.capturedTexts();

					QString author = list.at(1).toUtf8();
					QString message = list.at(2).toUtf8();

					c->renderMessage(author, message);

				}
				
			} else {

				ui.labelLoginState->setText("Votre mot de passe ou nom d'utilisteur \nest incorrect");
				ui.labelLoginState->setStyleSheet("QLabel { color : red; }");

			}

		} break;

		case 3: {

			if ( logged == 1 ) {

				QRegExp rx("pseudo:(.+)message:(.+)$");
				int codePos = rx.indexIn( str );
				QStringList list = rx.capturedTexts();

				QString author = list.at(1).toUtf8();
				QString message = list.at(2).toUtf8();

				c->renderMessage( author, message );

			}
				 

		}

		default:
			break;
		}

}

void Login::oauth() {

	if ( socket->state() == QTcpSocket::ConnectedState ) {

		if ( ui.lineEditLogin->text().size() == 0 )
			return ui.labelLoginState->setText( "Vous devez indiquer votre login..." );

		if ( ui.lineEditPassword->text().size() == 0 )
			return ui.labelLoginState->setText( "Vous devez indiquer votre mot de passe..." );

		ui.labelLoginState->setText( "Connected..." );

		QByteArray loginSalt(QCryptographicHash::hash(QByteArray(ui.lineEditLogin->text().toStdString().c_str()), QCryptographicHash::Sha256).toHex());
		QByteArray passSalt(QCryptographicHash::hash(QByteArray(ui.lineEditPassword->text().toStdString().c_str()), QCryptographicHash::Sha256).toHex());

		QString frmd  = "code:01login:" + loginSalt + "password:" + passSalt;

		qDebug() << frmd;

		socket->write( frmd.toUtf8() );

	}

}

void Login::regist() {

	if ( socket->state() == QTcpSocket::ConnectedState ) {

		if ( ui.lineEditLogin->text().size() == 0 )
			return ui.labelLoginState->setText( "Vous devez indiquer votre login..." );

		if ( ui.lineEditPassword->text().size() == 0 )
			return ui.labelLoginState->setText( "Vous devez indiquer votre mot de passe..." );

		if (ui.lineEditPseudo->text().size() == 0)
			return ui.labelLoginState->setText("Vous devez indiquer votre pseudo...");

		ui.labelLoginState->setText( "Connected..." );

		QByteArray loginSalt(QCryptographicHash::hash(QByteArray(ui.lineEditLogin->text().toStdString().c_str()), QCryptographicHash::Sha256).toHex());
		QByteArray passSalt(QCryptographicHash::hash(QByteArray(ui.lineEditPassword->text().toStdString().c_str()), QCryptographicHash::Sha256).toHex());

		QString frmd = "code:02login:" + loginSalt + "password:" + passSalt + "pseudo:" + ui.lineEditPseudo->text().toStdString().c_str();

		socket->write(frmd.toUtf8());

	}

}


void Login::switchState() {

	if ( state == 0 ) {

		ui.lineEditPseudo->setVisible(false);
		ui.labelPseudo->setVisible(false);
		ui.pushButtonRegister->setText("Se connecter");

		state = 1;

	}
	else {

		ui.lineEditPseudo->setVisible(true);
		ui.labelPseudo->setVisible(true);
		ui.pushButtonRegister->setText("S'inscrire");

		state = 0;

	}

}


void Login::handlerAction() {

	state == 0 ? Login::Login() : Login::oauth();

}

Login::~Login() {
}
