#include "client.h"

client::client( QWidget *parent ) : QMainWindow( parent ) {

    ui.setupUi( this );

	socket = new QTcpSocket( this );

	QObject::connect( socket, SIGNAL( connected() ), this, SLOT (onSocketConnected()  ));
	QObject::connect( socket, SIGNAL( disconnected() ), this, SLOT( onSocketDisconnected() ) );
	QObject::connect( socket, SIGNAL( readyRead() ), this, SLOT( onSocketReadyRead() ) );

	// 192.168.65.103
	socket->connectToHost( "127.0.0.1", 4456 );

	test = new QVBoxLayout( ui.scrollAreaWidgetContents );

}

void client::onSocketConnected() {

	authW = new auth( socket, NULL );
	authW->show();

};

void client::onSocketDisconnected() {

};

void client::onSocketReadyRead() {

	QByteArray res = socket->read( socket->bytesAvailable() );
	QString str( res );

	qDebug() << str;

	// —— Récupére le code de la réponse du serveur
	QRegExp rx( "^code:(\\d{0,2})"	);
	int codePos = rx.indexIn( str );
	QStringList list = rx.capturedTexts();

	// —— Si vous voyez ce message, c'est que j'ai oublié de l'enlever
	// qDebug() << str;

	// —— Handler des différents code | J'aime pas les switch, mais bon ...
	switch ( list.at( 1 ).toInt() ) {

		// —— Message serveur :: réception de la clé de chiffrage ( c bankal jé sé )
		case 0: {

			QRegExp rx( "key:(.{12})" );
			int keyPos = rx.indexIn( str );
			QStringList keyList = rx.capturedTexts();

			if ( keyPos == -1 )
				exit( 1 );

			this->key = keyList.at( 1 );

		} break;

		// —— Message serveur :: réponse à une connexion ou une inscription 
		case 1: {

			QRegExp rx( "ID:(\\d+)" );
			int codePos = rx.indexIn(str);
			QStringList list = rx.capturedTexts();

			if ( userID ) {
	
				QRegExp rx( "pseudo:(.+)" );
				int pseudoPos = rx.indexIn(str);
				QStringList pseudo = rx.capturedTexts();
				
				QLabel *labelMessage	 = new QLabel( "   —— " +  pseudo.at( 1 ).toUtf8() + " vient de se connecter" );
				labelMessage->setStyleSheet( "QLabel { color : #585c61 }");

				QVBoxLayout *layout		 = new QVBoxLayout();
				QFrame *messageContainer = new QFrame;

				test->addWidget( labelMessage ); 

				return;

			}

			if ( list.at( 1 ).toInt() != 0 ) {

				userID = list.at( 1 ).toInt();

				authW->close();
				this->show();

				// ——— Récupére les 100 derniers messages

				// —— Découpe le block massif de message en tableau de messages
				QStringList lastMessagses = str.split( QString( "code:03" ) );

				// —— Boucle sur le tableau des messages
				for ( QString message : lastMessagses ) {

					// —— Notre message est découpé de la sorte "code:01pseudo:joemessage:le Joe, le David LA CALOTTE DE SES MORTS....
 					QRegExp rx( "pseudo:(.+)message(.+)$" );
					int codePos = rx.indexIn( message );
					QStringList list = rx.capturedTexts();

					QString author = list.at( 1 ).toUtf8();
					
					this->renderMessage( author, 
						this->decrypt( )
						);

				}

			} else authW->statusExterne( "Nom d'utilisateur ou mot de passe invalide" );

		} break; 

		// —— Message serveur :: Nouveau message
		case 3: {

			if ( !userID )
				return;

			// —— Récupére les éléments du messages
			QRegExp rx( "pseudo:(.+)message:(.+)$" );
			int codePos = rx.indexIn( str );
			QStringList list = rx.capturedTexts();

			QString author  = list.at( 1 ).toUtf8()
				  , message = list.at( 2 ).toUtf8();

			this->renderMessage( author, message );

		}

	}

};

void client::renderMessage( QString author, QString message ) {

	QGroupBox *groupBox		 = new QGroupBox( author );
	QLabel *labelMessage	 = new QLabel( message );
	QVBoxLayout *layout		 = new QVBoxLayout();
	QFrame *messageContainer = new QFrame;

 	layout->addWidget( labelMessage );
	groupBox->setLayout( layout );
	test->addWidget( groupBox ); 

}

void client::sendMessage() {

	if ( ui.lineMessage->text().size() == 0 )
		return;

	QByteArray encrytedMessage = this->encrypt(
		ui.lineMessage->text().toUtf8(),
		this->key
	);

	QByteArray response;
	response.append( "code:03ID" + QString::number( userID ) + "message:" );
	response.append( encrytedMessage );

	socket->write( response );

}

void client::sendImage() {

	QString filePath = QFileDialog::getOpenFileName(
		this, tr( "Image à envoyer" ), "C:/Users/Asgarrrr/Pictures", tr( "Image Files (*.png *.jpg *.bmp)" ) 
	);


	QImage image( filePath );
	QByteArray ba;
	QBuffer buffer( &ba );
	image.save( &buffer, "PNG" );

	socket->write( ba.toHex() );

}

QByteArray client::encrypt( QByteArray message, QString key ) {

	QByteArray messageEncrypted;

	for( int i = 0; i < message.size(); i++ )
		messageEncrypted.append( 
			message.toStdString()[ i ] ^ key.toStdString()[ i%key.size() ] 
		);

	return messageEncrypted;

}

QByteArray client::decrypt( QByteArray message, QString key ) {

	return this->encrypt( message, key );

}