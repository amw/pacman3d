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
      QGLFormat( QGL::AlphaChannel | QGL::SampleBuffers | QGL::AccumBuffer ),
      parent
    ),
    board( "original" ),
    hero( & this->board ),
    motionBlurFrames( 0 ),
    background( QColor::fromRgbF( 0.2f, 0.2f, 0.2f ) ),
    centerCamera( true ),
    cameraZoom( 1.0f )
{
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

  this->hero.setPosition( this->board.getPlayer1Start() );
  this->hero.setVelocity( 3.0f, 0 );

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

  glClear( GL_ACCUM_BUFFER_BIT );
}

void Game::paintGL() {
  this->printFpsReport();

  if ( ! this->lastFrame.isValid() ) {
    this->lastFrame.start();
  }

  if ( this->motionBlurFrames ) {
    this->paintWithMotionBlur( this->lastFrame.restart() );
  }
  else {
    this->paintFrame( this->lastFrame.restart() );
  }

  qApp->postEvent(
    this, new QPaintEvent( QRegion() ), Qt::LowEventPriority - 100
  );

  ++this->framesRenderedSinceLastReport;
}

void Game::paintWithMotionBlur( int timeStep ) {
  int frames = this->motionBlurFrames;
  if ( frames > timeStep ) {
    frames = timeStep;
  }

  double accumFactor = 1.0f / (double) ( frames + 1 );
  glAccum( GL_MULT, accumFactor );

  int timeLeft = timeStep;
  for ( int framesLeft = frames; framesLeft; --framesLeft ) {
    int step = timeLeft / framesLeft;

    this->paintFrame( step );
    glAccum( GL_ACCUM, accumFactor );

    timeLeft -= step;
  }

  glAccum( GL_RETURN, 1.0f );
}

void Game::paintFrame( int timeStep ) {
  this->hero.updatePosition( timeStep );

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  this->refreshCamera();

  this->board.render( *this );
  this->hero.render( *this );
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
    QPointF pacman( this->hero.getPosition() );

    gluLookAt(
      pacman.x() + 4.0f * this->cameraZoom,
      pacman.y() - 8.0f * this->cameraZoom,
      15.0f * this->cameraZoom,
      pacman.x(), pacman.y(), 0.0f,
      0.0f, 0.0f, 1.0f
    );
  }
}

void Game::keyPressEvent( QKeyEvent* event ) {
  if ( event->key() == Qt::Key_Left ) {
    this->hero.setDesiredDirection( QPoint( -1, 0 ) );
    event->accept();
  }
  else if ( event->key() == Qt::Key_Right ) {
    this->hero.setDesiredDirection( QPoint( 1, 0 ) );
    event->accept();
  }
  else if ( event->key() == Qt::Key_Up ) {
    this->hero.setDesiredDirection( QPoint( 0, 1 ) );
    event->accept();
  }
  else if ( event->key() == Qt::Key_Down ) {
    this->hero.setDesiredDirection( QPoint( 0, -1 ) );
    event->accept();
  }
  else if ( event->key() == Qt::Key_0 ) {
    this->cameraZoom = 1.0f;
    event->accept();
  }
  else if ( event->key() == Qt::Key_Equal ) {
    this->cameraZoom -= 0.025f;
    if ( this->cameraZoom < 0.4f ) {
      this->cameraZoom = 0.4f;
    }
    event->accept();
  }
  else if ( event->key() == Qt::Key_Minus ) {
    this->cameraZoom += 0.025f;
    if ( this->cameraZoom > 1.5f ) {
      this->cameraZoom = 1.5f;
    }
    event->accept();
  }
  else if ( event->key() == Qt::Key_C ) {
    if ( this->centerCamera ) {
      this->cameraZoom = 1.0f;
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
  else if ( event->key() == Qt::Key_N ) {
    if ( this->motionBlurFrames ) {
      --this->motionBlurFrames;
      if ( ! this->motionBlurFrames ) {
        qDebug() << "Turning off motion blur.";
      }
      else {
        qDebug() << "Decreasing motion blur frames to"
                 << this->motionBlurFrames;
      }
    }
    event->accept();
  }
  else if ( event->key() == Qt::Key_M ) {
    if ( ! this->motionBlurFrames ) {
      qDebug() << "Turning on motion blur.";
      glClear( GL_ACCUM_BUFFER_BIT );
    }
    else {
      qDebug() << "Increasing motion blur frames to" << this->motionBlurFrames;
    }
    ++this->motionBlurFrames;
    event->accept();
  }
  else {
    event->ignore();
  }
}

