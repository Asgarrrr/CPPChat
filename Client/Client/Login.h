#pragma once

#include <QWidget>
#include "ui_Login.h"
#include "Client.h"

class Login : public QWidget {
	Q_OBJECT

	public:
		Login(QWidget *parent = Q_NULLPTR);
		~Login();
		int ID;
		Client * c;
		int logged = 0;

	private:
		Ui::Login ui;
		QTcpSocket * socket;

	public slots:
		void onSocketConnected();
		void onSocketDisconnected();
		void onSocketReadyRead();

		void oauth();
		void regist();




};
