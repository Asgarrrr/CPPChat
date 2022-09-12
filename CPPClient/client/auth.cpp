#include "auth.h"

auth::auth( QTcpSocket * socket, QWidget *parent) : QWidget(parent) {
	
	ui.setupUi( this );

	this->socket = socket;

	// -- N'affiche pas le label et lineedit du pseudo au lancement
	ui.labelPseudo->setVisible(false);
	ui.linePseudo->setVisible(false);

}

auth::~auth() {
}

void auth::switchAction() {

	if ( actionSwitch == 1 ) {

		ui.linePseudo->setVisible( false );
		ui.labelPseudo->setVisible( false );
		ui.pushAction->setText( "Se connecter" );

		actionSwitch = 0;

	} else {

		ui.linePseudo->setVisible( true );
		ui.labelPseudo->setVisible( true );
		ui.pushAction->setText( "S'inscrire" );

		actionSwitch = 1;

	}

}

void auth::action() {

	actionSwitch == 0 ? auth::login() : auth::signUp();

}

void auth::login() {

	if ( socket->state() == QTcpSocket::ConnectedState ) {

		if ( ui.lineLogin->text().size() == 0 )
			return ui.labelState->setText( "Vous devez indiquer votre login..." );

		if ( ui.linePass->text().size() == 0 )
			return ui.labelState->setText( "Vous devez indiquer votre mot de passe..." );

		ui.labelState->setText( "En cours d'authentification ..." );

		QByteArray loginSalt = ( 
			QCryptographicHash::hash(
				QByteArray( ui.lineLogin->text().toStdString().c_str() ), 
				QCryptographicHash::Sha256
			).toHex() );

		QByteArray passSalt = (
			QCryptographicHash::hash(
				QByteArray( ui.linePass->text().toStdString().c_str() ),
				QCryptographicHash::Sha256
			).toHex() );

		QString request = "code:01login:" + loginSalt + "password:" + passSalt;

		socket->write( request.toUtf8() );

	}

}

void auth::signUp() {

	// —— Vérifie que la connexion est bien établie
	if  (socket->state() == QTcpSocket::ConnectedState ) {

		// —— Vérifie que les informations sont correctes
		if ( ui.lineLogin->text().size() == 0 )
			return ui.labelState->setText( "Vous devez indiquer votre login..." );

		if (ui.linePass->text().size() == 0)
			return ui.labelState->setText("Vous devez indiquer votre mot de passe...");

		if (ui.linePseudo->text().size() == 0)
			return ui.labelState->setText("Vous devez indiquer votre pseudo...");

		ui.labelState->setText( "En cours d'inscrciptionn ...");

		// ——  Hashage du couple login / mot de passe
		QByteArray loginSalt = (
			QCryptographicHash::hash(
				QByteArray(ui.lineLogin->text().toStdString().c_str()),
				QCryptographicHash::Sha256
			).toHex());

		QByteArray passSalt = (
			QCryptographicHash::hash(
				QByteArray(ui.linePass->text().toStdString().c_str()),
				QCryptographicHash::Sha256
			).toHex());

		QString pseudo  = ui.linePseudo->text().toStdString().c_str()

			  , request = "code:02login:" + loginSalt + "password:" + passSalt + "pseudo:" + pseudo;

		socket->write( request.toUtf8() );

		qDebug() << request;

	}

}

void auth::statusExterne( QString message ) {

	ui.labelState->setText( message );


}