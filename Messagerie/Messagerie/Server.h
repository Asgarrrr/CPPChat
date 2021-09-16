#pragma once

#include <QObject>
#include <QtWebSockets/QtWebSockets>
#include <qtcpsocket.h>
#include <qtcpserver.h>

class Server : public QObject
{
	Q_OBJECT

	public:
		Server( quint16 port, QObject *parent = Q_NULLPTR );

	private:
		QWebSocket *ws;

};
