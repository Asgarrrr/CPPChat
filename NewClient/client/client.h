#pragma once

#include <QtWidgets/QMainWindow>

#include <QDebug>
#include <QRegExp>
#include <QMainWindow>
#include <QFileDialog>
#include <qtcpsocket.h>
#include <qgroupbox.h>
#include <QVBoxLayout>
#include <QLabel>
#include <qbuffer.h>
	
#include "ui_client.h"
#include "auth.h"

class client : public QMainWindow {
    Q_OBJECT

	public:
		client( QWidget *parent = Q_NULLPTR );
		QByteArray client::encrypt( QByteArray string, QString key );
		QByteArray client::decrypt( QByteArray string, QString key );
		QTcpSocket * socket;
		auth *authW;
		int userID;
		QLayout* test;
		int pack = 0;

	private:
		Ui::clientClass ui;
		QString key;

	public slots:
		void onSocketConnected();
		void onSocketDisconnected();
		void onSocketReadyRead();
		void renderMessage(QString author, QString message);
		void sendMessage();
		void sendImage();


};
