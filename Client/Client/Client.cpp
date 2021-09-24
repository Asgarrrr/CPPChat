#include "Client.h"
#include <QDebug>
#include <QRegExp>
#include <QMainWindow>
#include <QCryptographicHash>
#include <QFileDialog>

Client::Client( QTcpSocket * socket, int ID, QWidget *parent) : QMainWindow(parent) {

	ui.setupUi(this);

	this->socket = socket;
	this->ID = ID;

	qDebug() << this->ID << ID; 
 	    
}

void Client::renderMessage(QString author, QString message) {

	ui.textEdit->append( author + ":" + message );

}

void Client::sendMessage( ) {

	if (ui.lineMessageSend->text().size() == 0)
		return;

	QString message = ui.lineMessageSend->text();
	QString response = "code:03ID:" + QString::number( ID ) + "message:" + message;

	ui.lineMessageSend->clear();

	socket->write( response.toUtf8() );


}

