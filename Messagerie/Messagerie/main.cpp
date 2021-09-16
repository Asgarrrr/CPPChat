<<<<<<< HEAD
#include "Client.h"
#include "Server.h"

#include <QtWidgets/QApplication>

int main( int argc, char *argv[] ) {

    QApplication a( argc, argv );

	Server S( 4511 );
	Client C;
    C.show();

    return a.exec();

=======
#include "Messagerie.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Messagerie w;
    w.show();
    return a.exec();
>>>>>>> c6d38892bca23b06e49996e47d1a323c6af4b57e
}
