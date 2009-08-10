#include "PacMan.hpp"


struct XYZ {
  double x;
  double y;
  double z;
};

PacMan::PacMan( GameBoard * board )
  : MovingObject( board )
{
}

PacMan::~PacMan() {
  glDeleteLists( this->sphereList, 1 );
  gluDeleteQuadric( this->sphereQuadric );
}

void PacMan::initializeGL( QGLWidget & target ) {
  QImage pacman( "textures/pacman.png" );
  this->pacmanTexture = target.bindTexture( pacman );

  this->sphereList = glGenLists( 1 );
  this->sphereQuadric = gluNewQuadric();
  gluQuadricTexture( this->sphereQuadric, true );

  glNewList( this->sphereList, GL_COMPILE );
  {
    glBindTexture( GL_TEXTURE_2D, this->pacmanTexture );
    glRotatef( 180, 1.0f, 0.0f, 0.0f );
    gluSphere( this->sphereQuadric, 0.4f, 360 / 5, 180 / 5 );
  }
  glEndList();
}

void PacMan::render( QGLWidget & ) {
  glPushMatrix();
  glTranslatef(
    this->position.x(),
    this->position.y(),
    0.5f
  );
  glCallList( this->sphereList );
  glPopMatrix();
}

bool PacMan::canAccess( int x, int y ) {
  return this->board->isAccessibleByPlayer( x, y );
}

void PacMan::setDesiredDirection( QPoint direction ) {
  if ( this->direction == direction ) {
    this->desiredDirection = QPoint();
    return;
  }
  else if ( - this->direction == direction ) {
    this->setDirection( direction );
    this->desiredDirection = QPoint();
  }
  else {
    this->desiredDirection = direction;
  }
}

void PacMan::atBlockCenter() {
  if ( this->desiredDirection.isNull() ) {
    return;
  }

  QPointF testPosition = this->position + this->desiredDirection;
  if ( this->canAccess( testPosition.x(), testPosition.y() ) ) {
    this->position.setX( this->alignToCenter( this->position.x() ) );
    this->position.setY( this->alignToCenter( this->position.y() ) );

    this->direction = this->desiredDirection;
    this->desiredDirection = QPoint();
  }
}

