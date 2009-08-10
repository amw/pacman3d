#include "MovingObject.hpp"
#include <math.h>


MovingObject::MovingObject( GameBoard * board )
  : velocity( 0.0f )
{
  this->board = board;
  this->lastPositionUpdate.start();
}

const QPointF & MovingObject::getPosition() const {
  return this->position;
}

const QPoint & MovingObject::getDirection() const {
  return this->direction;
}

double MovingObject::getVelocity() const {
  return this->velocity;
}

void MovingObject::setPosition( QPointF position ) {
  this->lastPositionUpdate.restart();
  this->position = position;
}

void MovingObject::setDirection( QPoint direction ) {
  this->updatePosition();
  this->direction = direction;
}

void MovingObject::setVelocity( double velocity ) {
  this->updatePosition();
  this->velocity = velocity;
}

bool MovingObject::testMovement( double posX, double posY ) {
  posX += this->direction.x() * 0.5f;
  posY += this->direction.y() * 0.5f;

  if ( ! this->canAccess( posX, posY ) ) {
    return false;
  }
  else {
    return true;
  }
}

void MovingObject::updatePosition() {
  int msElapsed = this->lastPositionUpdate.restart();

  if ( MINIMUM_VELOCITY > this->velocity ) {
    return;
  }

  double timeFactor = msElapsed / 1000.0f;

  double newX = this->position.x();
  double newY = this->position.y();

  newX += this->direction.x() * this->velocity * timeFactor;
  newY += this->direction.y() * this->velocity * timeFactor;

  if ( ! this->testMovement( newX, newY ) ) {
    // if you can't move, center at current block
    newX = this->alignToCenter( this->position.x() );
    newY = this->alignToCenter( this->position.y() );
  }

  this->position.setX( newX );
  this->position.setY( newY );

  this->testIfAtBlockCenter();
}

double MovingObject::alignToCenter( double position ) {
  double newPos = round( position * 2.0f );
  bool atCenter = (int) newPos % 2;
  newPos /= 2.0f;
  if ( ! atCenter ) {
    if ( newPos > position ) {
      newPos -= 0.5f;
    }
    else {
      newPos += 0.5f;
    }
  }
  return newPos;
}

void MovingObject::testIfAtBlockCenter() {
  double atXCenter = this->position.x() - (int) this->position.x();
  double atYCenter = this->position.y() - (int) this->position.y();

  atXCenter = fabs( atXCenter - 0.5f );
  atYCenter = fabs( atYCenter - 0.5f );

  if ( CENTER_ATTRACTION > atXCenter && CENTER_ATTRACTION > atYCenter ) {
    QPoint currentBlock( this->position.x(), this->position.y() );
    this->atBlockCenter();
  }
}

