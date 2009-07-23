#include "Game.hpp"
#include <QKeyEvent>

Game::Game( QWidget* parent )
  : QGLWidget( parent )
{
  this->background = QColor::fromRgbF( 0.5, 0.9, 0.5 );
  this->x = 0.0f;
  this->y = 0.0f;
  this->zoom = -10.0f;
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
  this->grabKeyboard();
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
  glFrustum( -1.0, +1.0, -1.0, +1.0, 5.0, 100.0 );
  glMatrixMode( GL_MODELVIEW );
}

void Game::paintGL() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();
  glTranslated( this->x, this->y, this->zoom );

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
    this->zoom += 0.3f;
  }
  else if ( event->key() == Qt::Key_Minus ) {
    this->zoom -= 0.3f;
  }
  else {
    event->ignore();
  }
  this->updateGL();
}

