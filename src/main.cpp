#include "GameWindow.hpp"

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

int main( int argc, char ** argv ) {
  QApplication app( argc, argv );
  QDir::setCurrent( QApplication::applicationDirPath() );

  GameWindow game( NULL );

  if ( ! game.initialize() ) {
    qDebug() << "Couldn't initialize game window.";
    return 0;
  }

  game.start();

  return app.exec();
}

