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

const QPointF & MovingObject::getDirection() const {
  return this->direction;
}

double MovingObject::getVelocity() const {
  return this->velocity;
}

void MovingObject::setPosition( QPointF position ) {
  this->lastPositionUpdate.restart();
  this->position = position;
}

void MovingObject::setDirection( QPointF direction ) {
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
    newX = round( this->position.x() * 2.0f ) / 2.0f;
    newY = round( this->position.y() * 2.0f ) / 2.0f;
  }

  this->position.setX( newX );
  this->position.setY( newY );
}

