#pragma once

#include <QtWidgets/QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <qtcpsocket.h>

#include "ui_Client.h"

class Client : public QMainWindow
{
    Q_OBJECT

	public:
		Client( QTcpSocket * socket, int ID, QWidget *parent = Q_NULLPTR);
		void renderMessage(QString author, QString message);

	private:
		Ui::ClientClass ui;
		QTcpSocket * socket;
		int ID;

	public slots:
		void sendMessage();
		// void browse();

};
