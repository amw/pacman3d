#include "Game.hpp"
#include <QApplication>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QRegion>
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
  this->printFpsReport();

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glTranslated( this->x, this->y, this->z );
  this->board.render( *this );

  glTranslatef( 0.0f, 1.5f, 0.5f );
  this->hero.render( *this );

  glFinish();

  qApp->postEvent(
    this, new QPaintEvent( QRegion() ), Qt::LowEventPriority - 100
  );

  ++this->framesRenderedSinceLastReport;
}

void Game::printFpsReport() {
  if ( ! this->lastFpsReport.isValid() ) {
    this->lastFpsReport.start();
    this->framesRenderedSinceLastReport = 0;
  }

  int msElapsed = this->lastFpsReport.elapsed();
  if ( msElapsed >= MS_TO_REPORT_FPS ) {
    float fps = this->framesRenderedSinceLastReport / (double) msElapsed;
    fps *= 1000.0f;
    qDebug( "FPS: %-6.2f", fps );

    this->lastFpsReport.restart();
    this->framesRenderedSinceLastReport = 0;
  }
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
}

