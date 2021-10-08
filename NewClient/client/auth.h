#pragma once

#include <QWidget>
#include <QDebug>
#include <QRegExp>
#include <QMainWindow>
#include <QCryptographicHash>
#include <QFileDialog>
#include <qtcpsocket.h>

#include "ui_auth.h"

class auth : public QWidget {
		Q_OBJECT

	public:
		auth(QTcpSocket * socket, QWidget *parent = Q_NULLPTR);
		~auth();
		void login();
		void signUp();
		void statusExterne( QString message );

	private:
		Ui::auth ui;
		QTcpSocket * socket;
		int actionSwitch = 0;

	public slots:
		void switchAction();
		void action();


};
