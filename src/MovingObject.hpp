#ifndef MOVINGOBJECT_HPP
#define MOVINGOBJECT_HPP

#include "GameBoard.hpp"
#include <QPointF>
#include <QPoint>
#include <QObject>
#include <QTime>

#define MINIMUM_VELOCITY 0.05f

class MovingObject : public QObject {
  Q_OBJECT

  public:
    MovingObject( GameBoard * board );
    const QPointF & getPosition() const;
    const QPoint & getDirection() const;
    double getVelocity() const;
    void setPosition( QPointF position );
    void setDirection( QPoint direction, int timeStep );
    void setVelocity( double velocity, int timeStep );
    void updatePosition( int timeStep );
    bool collidesWith( const MovingObject & obj );

  protected:
    double alignToCenter( double position );
    virtual bool canAccess( int x, int y ) = 0;
    virtual bool crossingBlockCenter( int timeStep, QPointF newPosition ) = 0;

  private:
    bool testMovement( QPointF newPosition );

  protected:
    GameBoard * board;
    QPointF position;
    QPoint direction;
    QPoint lastDirection;
    double velocity;
    double radius;
};

#endif

