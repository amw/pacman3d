#include "MovingObject.hpp"


MovingObject::MovingObject( GameBoard * board ) {
  this->board = board;
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

