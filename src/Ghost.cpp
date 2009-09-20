#include "Ghost.hpp"
#include <QList>

Ghost::Ghost( GameBoard * board, double r, double g, double b )
  : MovingObject( board )
{
  this->light.setDiffuse( 3.0f * r, 3.0f * g, 3.0f * b );
  this->light.setSpecular( 3.0f * r, 3.0f * g, 3.0f * b );
  this->light.setAttenuation( 1.0f, 1.0f, 3.0f );

  this->material.setEmission( r, g, b );
  this->material.setShininess( 30.0f );
}

Ghost::~Ghost() {
  glDeleteLists( this->sphereList, 1 );
  gluDeleteQuadric( this->sphereQuadric );
}

void Ghost::initializeGL( QGLWidget & target ) {
  QImage ghost( "textures/ghost.png" );
  this->ghostTexture = target.bindTexture( ghost );

  this->sphereList = glGenLists( 1 );
  this->sphereQuadric = gluNewQuadric();
  gluQuadricTexture( this->sphereQuadric, true );

  glNewList( this->sphereList, GL_COMPILE );
  {
    glBindTexture( GL_TEXTURE_2D, this->ghostTexture );
    this->material.updateGlState( Material::Front );
    glRotatef( 180, 1.0f, 0.0f, 0.0f );
    gluSphere( this->sphereQuadric, 0.3f, 360 / 5, 180 / 5 );
  }
  glEndList();
}

void Ghost::render( QGLWidget & ) {
  glPushMatrix();
  glTranslatef(
    this->position.x(),
    this->position.y(),
    0.5f
  );
  glCallList( this->sphereList );
  glPopMatrix();
}

void Ghost::updateGlLight( GLenum light ) {
  this->light.setPosition( this->position.x(), this->position.y(), 0.5f );
  this->light.updateGlState( light );
}

QPoint Ghost::getNewDirection() {
  QList< QPoint > directions;
  directions.append( QPoint(  1,  0 ) );
  directions.append( QPoint( -1,  0 ) );
  directions.append( QPoint(  0,  1 ) );
  directions.append( QPoint(  0, -1 ) );

  QPoint reverse = - this->direction;

  int i = 0;
  while ( i < directions.size() ) {
    QPointF testDirection = this->position;
    testDirection += directions.at( i );
    if ( directions.at( i ) == reverse ) {
      directions.removeAt( i );
    }
    else if ( this->canAccess( testDirection.x(), testDirection.y() ) ) {
      ++i;
    }
    else {
      directions.removeAt( i );
    }
  }

  if ( ! directions.size() ) {
    return reverse;
  }

  return directions.at( qrand() % directions.size() );
}

bool Ghost::canAccess( int x, int y ) {
  return this->board->isAccessibleByGhost( x, y );
}

bool Ghost::crossingBlockCenter( int, QPointF newPosition ) {
  QPoint newDirection = this->getNewDirection();

  if ( this->direction == newDirection ) {
    return false;
  }

  this->direction = newDirection;

  QPointF center(
    this->alignToCenter( newPosition.x() ),
    this->alignToCenter( newPosition.y() )
  );

  QPointF difference = newPosition - center;
  double dX = fabs( difference.x() );
  double dY = fabs( difference.y() );
  double distance = fmax( dX, dY );

  this->position = center + QPointF( this->direction ) * distance;

  return true;
}

