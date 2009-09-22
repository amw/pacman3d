#include "GameWindow.hpp"
#include "Game.hpp"
#include <QKeyEvent>
#include <QHBoxLayout>

GameWindow::GameWindow( QWidget* parent )
  : QWidget( parent ),
    game( 0 ),
    lastSize( 400, 400 )
{
  this->setWindowTitle( trUtf8( "Pacman 3d" ) );

  this->setContentsMargins( -20, -20, -20, -5 );
  this->layout = new QHBoxLayout();
  this->setLayout( this->layout );
}

GameWindow::~GameWindow() {
  delete this->game;
}

bool GameWindow::initialize() {
  this->game = new Game( this );
  this->layout->addWidget( this->game );

  if ( ! this->game->initialize() ) {
    return false;
  }

  this->game->setFocus( Qt::ActiveWindowFocusReason );
  this->show();
  this->raise();
  this->activateWindow();

  this->game->start();

  return true;
}

QSize GameWindow::minimumSizeHint() const {
  return QSize( 200, 200 );
}

QSize GameWindow::sizeHint() const {
  return this->lastSize;
}

void GameWindow::keyPressEvent( QKeyEvent* event ) {
  if ( event->key() == Qt::Key_F ) {
    this->game->clearFocus();
    if ( this->isFullScreen() ) {
      this->showNormal();
      this->setContentsMargins( -20, -20, -20, -5 );
      this->activateWindow();
    }
    else {
      this->lastSize = this->size();
      this->showFullScreen();
      this->setContentsMargins( -20, -20, -20, -20 );
    }
    this->game->setFocus( Qt::ActiveWindowFocusReason );

    event->accept();
    return;
  }

  event->ignore();
}

