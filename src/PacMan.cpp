#include "PacMan.hpp"


PacMan::PacMan( GameBoard * board )
  : MovingObject( board )
{
  this->material.setSpecular( 1.0f, 1.0f, 1.0f );
  this->material.setShininess( 50.0f );
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
    this->material.updateGlState( Material::Front );
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
    this->direction = direction;
    this->desiredDirection = QPoint();
  }
  else if ( this->direction.isNull() ) {
    this->direction = direction;
    this->desiredDirection = QPoint();
  }
  else {
    this->desiredDirection = direction;
  }
}

bool PacMan::crossingBlockCenter( int, QPointF newPosition ) {
  if ( this->desiredDirection.isNull() ) {
    return false;
  }

  QPointF center(
    this->alignToCenter( newPosition.x() ),
    this->alignToCenter( newPosition.y() )
  );

  QPointF testPosition = center + this->desiredDirection;
  if ( this->canAccess( testPosition.x(), testPosition.y() ) ) {
    this->direction = this->desiredDirection;
    this->desiredDirection = QPoint();

    QPointF difference = newPosition - center;
    double dX = fabs( difference.x() );
    double dY = fabs( difference.y() );
    double distance = fmax( dX, dY );

    this->position = center + QPointF( this->direction ) * distance;

    return true;
  }

  return false;
}

