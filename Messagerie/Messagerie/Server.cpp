#include "Server.h"
#include <QDebug>
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"


Server::Server( quint16 port, QObject *parent ) : QObject( parent ) {

	ws = new QWebSocket( port );

}


}
