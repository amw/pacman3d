#include "MovingObject.hpp"


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

void MovingObject::updatePosition() {
  int msElapsed = this->lastPositionUpdate.restart();

  if ( MINIMUM_VELOCITY > this->velocity ) {
    return;
  }

  double moveFactor = msElapsed / 1000.0f;

  this->position.rx() += this->direction.x() * this->velocity * moveFactor;
  this->position.ry() += this->direction.y() * this->velocity * moveFactor;
}

