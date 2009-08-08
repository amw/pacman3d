#include "Game.hpp"
#include <QKeyEvent>
#include <QDebug>

Game::Game( QWidget* parent )
  : QGLWidget( parent ),
    board( "original" )
{
  this->background = QColor::fromRgbF( 0.1, 0.1, 0.1 );
  this->x = 0.0f;
  this->y = 0.0f;
  this->z = -30.f;
}

Game::~Game() {
  this->makeCurrent();
}

QSize Game::minimumSizeHint() const {
  return QSize( 200, 200 );
}

QSize Game::sizeHint() const {
  return QSize( 400, 400 );
}

bool Game::initialize() {
  if ( ! this->board.initialize() ) {
    return false;
  }

  return true;
}

void Game::start() {
  this->grabKeyboard();
}

void Game::initializeGL() {
  this->qglClearColor( this->background );
  glShadeModel( GL_FLAT );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );
  glEnable( GL_TEXTURE_2D );
  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

  this->board.initializeGL( *this );
  this->hero.initializeGL( *this );
}

void Game::resizeGL( int width, int height ) {
  glViewport( 0, 0, width, height );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  gluPerspective( 45, (double) width / (double) height, 0.5f, 35.0f );

  gluLookAt(
    5.0f, -5.0f, -25.0f,
    0.0f,  0.0f, -28.0f,
    0.0f,  0.0f,   1.0f
  );
}

void Game::paintGL() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glTranslated( this->x, this->y, this->z );
  this->board.render( *this );

  glTranslatef( 0.0f, 0.0f, 2.0f );
  this->hero.render( *this );
}

void Game::keyPressEvent( QKeyEvent* event ) {
  if ( event->key() == Qt::Key_Left ) {
    this->x -= 0.1f;
    event->accept();
  }
  else if ( event->key() == Qt::Key_Right ) {
    this->x += 0.1f;
    event->accept();
  }
  else if ( event->key() == Qt::Key_Up ) {
    this->y += 0.1f;
    event->accept();
  }
  else if ( event->key() == Qt::Key_Down ) {
    this->y -= 0.1f;
    event->accept();
  }
  else if ( event->key() == Qt::Key_Equal ) {
    this->z += 0.4f;
    qDebug() << "Setting zoom to:" << this->z;
  }
  else if ( event->key() == Qt::Key_Minus ) {
    this->z -= 0.4f;
    qDebug() << "Setting zoom to:" << this->z;
  }
  else {
    event->ignore();
  }
  this->updateGL();
}

