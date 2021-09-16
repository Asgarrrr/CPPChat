#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWebSockets/QtWebSockets>
#include "ui_Messagerie.h"

class Client : public QMainWindow
{
    Q_OBJECT

	public:
		Client(QWidget *parent = Q_NULLPTR);

	private:
		Ui::ClientClass ui;


};
