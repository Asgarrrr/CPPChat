#include "Client.h"
#include <QtWebSockets/QtWebSockets>

Client::Client(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}
