#include "MovingObject.hpp"
#include <math.h>


MovingObject::MovingObject( GameBoard * board ) :
  board( board ),
  velocity( 0.0f )
{
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
  this->position = position;
}

void MovingObject::setDirection( QPoint direction, int timeStep ) {
  this->updatePosition( timeStep );
  this->direction = direction;
}

void MovingObject::setVelocity( double velocity, int timeStep ) {
  this->updatePosition( timeStep );
  this->velocity = velocity;
}

bool MovingObject::testMovement( double posX, double posY ) {
  // can access target block?
  if ( ! this->canAccess( posX, posY ) ) {
    return false;
  }

  // can access every block on the path?
  if ( posX > this->position.x() ) {
    for ( double x = this->position.x() + 1.0f; x < posX; x += 1.0f ) {
      if ( ! this->canAccess( x, this->position.y() ) ) {
        return false;
      }
    }
  }
  else if ( posX < this->position.x() ) {
    for ( double x = this->position.x() - 1.0f; x > posX; x -= 1.0f ) {
      if ( ! this->canAccess( x, this->position.y() ) ) {
        return false;
      }
    }
  }

  if ( posY > this->position.y() ) {
    for ( double y = this->position.y() + 1.0f; y < posY; y += 1.0f ) {
      if ( ! this->canAccess( y, this->position.y() ) ) {
        return false;
      }
    }
  }
  else if ( posY < this->position.y() ) {
    for ( double y = this->position.y() - 1.0f; y > posY; y -= 1.0f ) {
      if ( ! this->canAccess( y, this->position.y() ) ) {
        return false;
      }
    }
  }

  return true;
}

void MovingObject::updatePosition( int timeStep ) {
  if ( MINIMUM_VELOCITY > this->velocity ) {
    return;
  }

  double timeFactor = timeStep / 1000.0f;

  double newX = this->position.x();
  double newY = this->position.y();

  newX += this->direction.x() * this->velocity * timeFactor;
  newY += this->direction.y() * this->velocity * timeFactor;

  if ( ! this->testMovement(
      newX + this->direction.x() * 0.5f,
      newY + this->direction.y() * 0.5f
  ) ) {
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
    this->atBlockCenter();
  }
}

