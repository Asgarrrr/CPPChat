#pragma once

#include <QWidget>
#include "ui_Login.h"
#include "Client.h"

class Login : public QWidget {
	Q_OBJECT

	public:
		Login(QWidget *parent = Q_NULLPTR);
		~Login();
		Client * c;

	private:
		Ui::Login ui;
		QTcpSocket * socket;
		int ID;
		int logged = 0;
		int state = 1;
		QString Pseudo;

	public slots:
		void onSocketConnected();
		void onSocketDisconnected();
		void onSocketReadyRead();
		void switchState();
		void oauth();
		void regist();
		void handlerAction();




};
