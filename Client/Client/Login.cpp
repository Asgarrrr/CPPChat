#include "Login.h"
#include "Client.h"
#include <qdebug.h>


Login::Login( QWidget *parent ) : QWidget( parent ) {

	socket = new QTcpSocket(this);

	ui.setupUi(this);
	ui.labelLoginState->setText("Connecting ...");

	QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));

	// IP GOGOL :: 192.168.65.103

	socket->connectToHost( "192.168.65.103", 4456 );
	
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
				
				ID = list.at( 1 ).toInt();

				c = new Client(socket, ID, NULL);
				ui.setupUi(this);
				c->show();
				logged = 1;
				this->close();

				QRegExp rx( "code:03.+?(?=code)" );
				int codePos = rx.indexIn( str );
				QStringList list = rx.capturedTexts();

				qDebug() << list;

				for (int i = 1; i < list.size(); ++i)
					qDebug() << list.at(i).toLocal8Bit().constData() << Qt::endl;
				
			} else {

				ui.labelLoginState->setText("Votre mot de passe ou nom d'utilisteur \nest incorrect");
				ui.labelLoginState->setStyleSheet("QLabel { color : red; }");

			}

		} break;

		case 3: {

			qDebug() << logged;

			if ( logged == 1 ) {

				QRegExp rx("pseudo:(.+)message:(.+)$");
				int codePos = rx.indexIn( str );
				QStringList list = rx.capturedTexts();

				QString author = list.at(1).toUtf8();
				QString message = list.at(2).toUtf8();

				qDebug() << "test";

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

		QString login = ui.lineEditLogin->text()
			  , pass  = ui.lineEditPassword->text()
			  , frmd  = "code:01login:" + login + "password:" + pass;

		socket->write( frmd.toUtf8() );

	}

}

void Login::regist() {

	if ( socket->state() == QTcpSocket::ConnectedState ) {

		if ( ui.lineEditLogin->text().size() == 0 )
			return ui.labelLoginState->setText( "Vous devez indiquer votre login..." );

		if ( ui.lineEditPassword->text().size() == 0 )
			return ui.labelLoginState->setText( "Vous devez indiquer votre mot de passe..." );

		ui.labelLoginState->setText( "Connected..." );

		QString login = ui.lineEditLogin->text()
			  , pass = ui.lineEditPassword->text()
			  , frmd = "code:02login:" + login + "password:" + pass;

		socket->write( frmd.toUtf8() );

	}

}


Login::~Login() {
}
