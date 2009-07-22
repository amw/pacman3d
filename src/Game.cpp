#include "Game.hpp"
#include <QKeyEvent>

Game::Game( QWidget* parent )
  : QGLWidget( parent )
{
  background = QColor::fromRgbF( 0.5, 0.9, 0.5 );
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
  return true;
}

void Game::start() {
}

void Game::initializeGL() {
  this->qglClearColor( this->background );
  glShadeModel( GL_FLAT );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );
}

void Game::resizeGL( int width, int height ) {
  int side = qMin( width, height );
  glViewport( ( width - side ) / 2, ( height - side ) / 2, side, side );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum( -0.5, +0.5, +0.5, -0.5, 4.0, 15.0 );
  glMatrixMode( GL_MODELVIEW );
}

void Game::paintGL() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();
  glTranslated( -0.5, -0.5, -5.0 );

  glBegin(GL_QUADS);
  {
    glColor3f( 1.0, 0.0, 0.0 );
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, -0.1f,  0.1f);
    glTexCoord2f(0.1f, 0.0f); glVertex3f( 0.1f, -0.1f,  0.1f);
    glTexCoord2f(0.1f, 0.1f); glVertex3f( 0.1f,  0.1f,  0.1f);
    glTexCoord2f(0.0f, 0.1f); glVertex3f(-0.1f,  0.1f,  0.1f);

    glColor3f( 0.0, 1.0, 0.0 );
    glTexCoord2f(0.1f, 0.0f); glVertex3f(-0.1f, -0.1f, -0.1f);
    glTexCoord2f(0.1f, 0.1f); glVertex3f(-0.1f,  0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.1f); glVertex3f( 0.1f,  0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.1f, -0.1f);

    glColor3f( 0.0, 0.0, 1.0 );
    glTexCoord2f(0.0f, 0.1f); glVertex3f(-0.1f,  0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f,  0.1f,  0.1f);
    glTexCoord2f(0.1f, 0.0f); glVertex3f( 0.1f,  0.1f,  0.1f);
    glTexCoord2f(0.1f, 0.1f); glVertex3f( 0.1f,  0.1f, -0.1f);

    glColor3f( 1.0, 0.0, 1.0 );
    glTexCoord2f(0.1f, 0.1f); glVertex3f(-0.1f, -0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.1f); glVertex3f( 0.1f, -0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.1f,  0.1f);
    glTexCoord2f(0.1f, 0.0f); glVertex3f(-0.1f, -0.1f,  0.1f);

    glColor3f( 1.0, 1.0, 1.0 );
    glTexCoord2f(0.1f, 0.0f); glVertex3f( 0.1f, -0.1f, -0.1f);
    glTexCoord2f(0.1f, 0.1f); glVertex3f( 0.1f,  0.1f, -0.1f);
    glTexCoord2f(0.0f, 0.1f); glVertex3f( 0.1f,  0.1f,  0.1f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.1f,  0.1f);

    glColor3f( 0.0, 1.0, 1.0 );
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, -0.1f, -0.1f);
    glTexCoord2f(0.1f, 0.0f); glVertex3f(-0.1f, -0.1f,  0.1f);
    glTexCoord2f(0.1f, 0.1f); glVertex3f(-0.1f,  0.1f,  0.1f);
    glTexCoord2f(0.0f, 0.1f); glVertex3f(-0.1f,  0.1f, -0.1f);
  }
  glEnd();
}

void Game::keyPressEvent( QKeyEvent* event ) {
  event->ignore();
}

