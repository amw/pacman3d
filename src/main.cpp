#include "GameWindow.hpp"

#include <QtCore>
#include <QtGui>

int main( int argc, char ** argv ) {
  QApplication app( argc, argv );
  QDir::setCurrent( QApplication::applicationDirPath() );

  GameWindow window( NULL );

  if ( ! window.initialize() ) {
    qDebug() << "Couldn't initialize game window.";
    return 0;
  }

  return app.exec();
}

