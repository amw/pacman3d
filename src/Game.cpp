#include "Game.hpp"
#include "gl.h"
#include <QApplication>
#include <QKeyEvent>
#include <QPaintEvent>
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
    timer( this ),
    motionBlurFrames( 0 ),
    background( QColor::fromRgbF( 0.2f, 0.2f, 0.2f ) ),
    centerCamera( true ),
    cameraZoom( 1.0f )
{
  this->initializeLights();

  QTime midnight( 0, 0 );
  qsrand( midnight.msecsTo( QTime::currentTime() ) );

  connect(
    &this->hero, SIGNAL( collectedPoint() ),
    this, SLOT( playerCollectedPoint() )
  );

  this->ghosts[ 0 ] = new Ghost( & this->board, 1.0f, 0.0f, 0.0f );
  this->ghosts[ 1 ] = new Ghost( & this->board, 0.9f, 0.5f, 0.9f );
  this->ghosts[ 2 ] = new Ghost( & this->board, 0.0f, 1.0f, 1.0f );
  this->ghosts[ 3 ] = new Ghost( & this->board, 1.0f, 1.0f, 0.0f );

  this->timer.setSingleShot( true );
  connect( & this->timer, SIGNAL( timeout() ), this, SLOT( updateGL() ) );
}

Game::~Game() {
  this->makeCurrent();
  for ( int i = 0; i < GHOSTS_COUNT; ++i) {
    delete this->ghosts[ i ];
  }
  delete this->shaderProgram;
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

  this->lifes = 3;
  this->points = 0;

  this->hero.setPosition( this->board.getPlayer1Start() );
  this->hero.setVelocity( 3.0f, 0 );

  int ghostStartCount = this->board.getGhostStarts().size();

  for ( int i = 0; i < GHOSTS_COUNT; ++i ) {
    QPointF startPos = this->board.getGhostStarts().at( i % ghostStartCount );
    this->ghosts[ i ]->setPosition( startPos );
    this->ghosts[ i ]->setVelocity( 3.0f, 0 );
    this->ghosts[ i ]->setDirection( this->ghosts[ i ]->getNewDirection(), 0 );
  }

  this->lastPlayerDeath = QTime::currentTime().addSecs( -60 );

  return true;
}

void Game::start() {
  this->grabKeyboard();
}

void Game::initializeGL() {
  GLenum err = glewInit();
  if ( GLEW_OK != err ) {
    qDebug() << "GLEW failed. Reason:";
    qDebug() << (char*) glewGetErrorString( err );
    qApp->exit( 1 );
  }

  this->qglClearColor( this->background );

  glEnable( GL_MULTISAMPLE );
  glEnable( GL_CULL_FACE );

  glDepthFunc( GL_LESS );
  glEnable( GL_DEPTH_TEST );

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glEnable( GL_TEXTURE_2D );
  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

  glEnable( GL_LIGHTING );

  if ( ! GLEW_VERSION_2_0 ) {
    qDebug() << "You don't have OpenGL 2.0. Expect the unexpected.";

    this->shadersAreSupported = false;
    this->usingShaders = false;
  }
  else {
    this->shadersAreSupported = true;
    this->usingShaders = true;
    this->initializeShaders();
    this->shaderProgram->enable( true );
  }

  this->board.initializeGL( *this );
  this->hero.initializeGL( *this );
  for ( int i = 0; i < GHOSTS_COUNT; ++i ) {
    this->ghosts[ i ]->initializeGL( *this );
  }
}

void Game::resizeGL( int width, int height ) {
  glViewport( 0, 0, width, height );

  this->aspectRatio = (double) width / (double) height;

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  gluPerspective( FOVY, this->aspectRatio, 0.5f, 50.0f );

  glMatrixMode( GL_MODELVIEW );
  this->needsRepaint = true;
}

void Game::paintGL() {
  this->printFpsReport();

  if ( ! this->lastFrame.isValid() ) {
    this->lastFrame.start();
  }

  if ( this->needsRepaint ) {
    this->paintFrame( this->lastFrame.restart() );
    glAccum( GL_LOAD, 1.0f );
    this->needsRepaint = false;
  }
  if ( this->motionBlurFrames ) {
    this->paintWithMotionBlur( this->lastFrame.restart() );
  }
  else {
    this->paintFrame( this->lastFrame.restart() );
  }

  ++this->framesRenderedSinceLastReport;

  this->timer.start();
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
  for ( int i = 0; i < GHOSTS_COUNT; ++i ) {
    this->ghosts[ i ]->updatePosition( timeStep );

    if ( this->ghosts[ i ]->collidesWith( this->hero ) ) {
      if ( this->lastPlayerDeath.elapsed() > INVULNERABILITY_TIME ) {
        this->lastPlayerDeath.restart();
        this->hero.setPosition( this->board.getPlayer1Start() );
        if ( --this->lifes < 0 ) {
          qDebug() << "Game over.";
          QCoreApplication::exit( 0 );
        }
        else {
          qDebug() << "You have" << this->lifes << "lives left.";
        }
      }
    }
  }

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  this->refreshCamera();
  this->prepareLights();

  this->board.render( *this );

  int sinceDeath = this->lastPlayerDeath.elapsed();
  if ( sinceDeath < INVULNERABILITY_TIME ) {
    int divider = 400;
    if ( sinceDeath > INVULNERABILITY_TIME * 0.66f ) {
      divider = 200;
    }

    sinceDeath /= divider;
    if ( ! ( sinceDeath % 2 ) ) {
      this->hero.render( *this );
    }
  }
  else {
    this->hero.render( *this );
  }

  for ( int i = 0; i < GHOSTS_COUNT; ++i ) {
    this->ghosts[ i ]->render( *this );
  }
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

void Game::initializeShaders() {
  this->shaderProgram = new PipelineProgram();
  this->shaderProgram->attachShader(
    GL_VERTEX_SHADER, "shaders/both.vert"
  );
  this->shaderProgram->attachShader(
    GL_FRAGMENT_SHADER, "shaders/both.frag"
  );
  this->shaderProgram->link();

  this->lightsCountLocation =
    this->shaderProgram->getUniformLocation( "lightsCount" );
}

void Game::initializeLights() {
  this->ambientLight[ 0 ] = 1.0f;
  this->ambientLight[ 1 ] = 1.0f;
  this->ambientLight[ 2 ] = 1.0f;
  this->ambientLight[ 3 ] = 1.0f;

  this->ghostStartsLight.setDiffuse( 1.0f, 1.0f, 0.7f );
  this->ghostStartsLight.setSpecular( 1.0f, 1.0f, 1.0f );
  this->ghostStartsLight.setAttenuation( 1.0f, 0.1f, 0.1f );
}

void Game::prepareLights() {
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, this->ambientLight );

  int light = 0;

  light += this->prepareMainLight( light );

  for ( int i = 0; i < GHOSTS_COUNT; ++i ) {
    this->ghosts[ i ]->updateGlLight( GL_LIGHT0 + light++ );
  }

  if ( this->usingShaders ) {
    glUniform1i( this->lightsCountLocation, light );
  }

  while ( light < 8 ) {
    glDisable( GL_LIGHT0 + light++ );
  }
}

int Game::prepareMainLight( int light ) {
  float mainLightX = 0.0f, mainLightY = 0.0f;

  QVector< QPointF > lights = this->board.getGhostStarts();
  QVector< QPointF >::const_iterator i;

  for ( i = lights.begin(); i != lights.end(); ++i ) {
    mainLightX += i->x();
    mainLightY += i->y();
  }

  mainLightX /= lights.size();
  mainLightY /= lights.size();

  glEnable( GL_LIGHT0 + light );
  this->ghostStartsLight.setPosition( mainLightX, mainLightY, 4.0f );
  //this->ghostStartsLight.setDirection( 0.0f, 0.0f, 1.0f );
  this->ghostStartsLight.updateGlState( GL_LIGHT0 + light );

  return 1;
}

void Game::playerCollectedPoint() {
  ++this->points;
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
      glAccum( GL_LOAD, 1.0f );
    }
    else {
      qDebug() << "Increasing motion blur frames to" << this->motionBlurFrames;
    }
    ++this->motionBlurFrames;
    event->accept();
  }
  else if ( event->key() == Qt::Key_S ) {
    if ( this->shadersAreSupported ) {
      if ( this->usingShaders ) {
        this->usingShaders = false;
        this->shaderProgram->enable( false );
        qDebug() << "Shaders turned off.";
      }
      else {
        this->usingShaders = true;
        this->shaderProgram->enable( true );
        qDebug() << "Shaders turned on.";
      }
    }
    else {
      qDebug() << "You need OpenGL 2.0 to use shaders.";
    }
    event->accept();
  }
  else {
    event->ignore();
  }
}

