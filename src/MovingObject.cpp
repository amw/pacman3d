#include "MovingObject.hpp"
#include <math.h>

#define MIN( v1, v2 ) ( ( v2 < v1 ) ? v2 : v1 )

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
  if ( this->direction == direction ) {
    return;
  }

  this->updatePosition( timeStep );
  this->direction = direction;
}

void MovingObject::setVelocity( double velocity, int timeStep ) {
  this->updatePosition( timeStep );
  this->velocity = velocity;
}

bool MovingObject::testMovement( QPointF newPosition ) {
  newPosition += QPointF( this->direction ) * 0.5f;
  double posX = newPosition.x();
  double posY = newPosition.y();

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

  if ( this->direction.isNull() ) {
    return;
  }

  double timeFactor = timeStep / 1000.0f;

  QPointF newPosition( this->direction );
  newPosition *= this->velocity * timeFactor;
  newPosition += this->position;

  int oldXHalfBlock = this->position.x() / 0.5f;
  int oldYHalfBlock = this->position.y() / 0.5f;
  int newXHalfBlock = newPosition.x() / 0.5f;
  int newYHalfBlock = newPosition.y() / 0.5f;

  bool crossingX =
    oldXHalfBlock != newXHalfBlock &&
    ( MIN( oldXHalfBlock, newXHalfBlock ) % 2 ) == 0;
  bool crossingY =
    oldYHalfBlock != newYHalfBlock &&
    ( MIN( oldYHalfBlock, newYHalfBlock ) % 2 ) == 0;

  if ( crossingX || crossingY ) {
    if ( this->crossingBlockCenter( timeStep, newPosition ) ) {
      return;
    }
  }

  if ( ! this->testMovement( newPosition ) ) {
    // if you can't move, center at current block
    newPosition.setX( this->alignToCenter( this->position.x() ) );
    newPosition.setY( this->alignToCenter( this->position.y() ) );
    this->direction = QPoint();
  }

  this->position = newPosition;
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

