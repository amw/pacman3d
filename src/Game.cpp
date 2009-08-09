#include "Game.hpp"
#include <QApplication>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QRegion>
#include <QDebug>
#include <math.h>

#define DEG_TO_RADS 0.0174532925f

Game::Game( QWidget* parent )
  : QGLWidget(
      QGLFormat( QGL::AlphaChannel | QGL::SampleBuffers ),
      parent
    ),
    board( "original" ),
    hero( & this->board )
{
  this->background = QColor::fromRgbF( 0.2, 0.2, 0.2 );
  this->centerCamera = true;
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

  glEnable( GL_MULTISAMPLE );
  glEnable( GL_CULL_FACE );

  glDepthFunc( GL_LESS );
  glEnable( GL_DEPTH_TEST );

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glEnable( GL_TEXTURE_2D );
  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

  this->board.initializeGL( *this );
  this->hero.initializeGL( *this );
}

void Game::resizeGL( int width, int height ) {
  glMatrixMode( GL_MODELVIEW );
  glViewport( 0, 0, width, height );

  this->aspectRatio = (double) width / (double) height;

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  gluPerspective( FOVY, this->aspectRatio, 0.5f, 50.0f );
}

void Game::paintGL() {
  this->printFpsReport();

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  this->refreshCamera();

  this->board.render( *this );
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

void Game::refreshCamera() {
  if ( this->centerCamera ) {
    QPointF boardSize = this->board.getRealSize();
    QPointF center( boardSize.x() / 2.0f, boardSize.y() / 2.0f );

    double cameraDistance = boardSize.y() / 2.0f;
    cameraDistance /= tanf( FOVY * DEG_TO_RADS / 2.0f );
    cameraDistance += 2.0f;

    gluLookAt(
      center.x(), center.y(), cameraDistance,
      center.x(), center.y(), 0.0f,
      0.0f, 1.0f, 0.0f
    );
  }
  else if ( this->isometricCamera ) {
    QPointF boardSize = this->board.getRealSize();
    QPointF center( boardSize.x() / 2.0f, boardSize.y() / 2.0f );

    gluLookAt(
      center.x() + 10.0f, center.y() - 10.0f, 15.0f,
      center.x(), center.y(), 0.0f,
      0.0f,  0.0f,  1.0f
    );
  }
}

void Game::keyPressEvent( QKeyEvent* event ) {
  if ( event->key() == Qt::Key_Left ) {
    event->accept();
  }
  else if ( event->key() == Qt::Key_Right ) {
    event->accept();
  }
  else if ( event->key() == Qt::Key_Up ) {
    event->accept();
  }
  else if ( event->key() == Qt::Key_Down ) {
    event->accept();
  }
  else if ( event->key() == Qt::Key_Equal ) {
    event->accept();
  }
  else if ( event->key() == Qt::Key_Minus ) {
    event->accept();
  }
  else if ( event->key() == Qt::Key_C ) {
    if ( this->centerCamera ) {
      this->centerCamera = false;
      this->isometricCamera = true;
    }
    else {
      this->centerCamera = true;
      this->isometricCamera = false;
    }
    this->refreshCamera();

    event->accept();
  }
  else {
    event->ignore();
  }
}

