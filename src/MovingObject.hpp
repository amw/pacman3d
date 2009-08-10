#ifndef MOVINGOBJECT_HPP
#define MOVINGOBJECT_HPP

#include "GameBoard.hpp"
#include <QPointF>
#include <QObject>
#include <QTime>

#define MINIMUM_VELOCITY 0.05f

class MovingObject : public QObject {
  Q_OBJECT

  public:
    MovingObject( GameBoard * board );
    const QPointF & getPosition() const;
    const QPointF & getDirection() const;
    double getVelocity() const;
    void setDirection( QPointF direction );
    void setPosition( QPointF position );
    void setVelocity( double velocity );
    void updatePosition();

  protected:
    virtual bool canAccess( int x, int y ) = 0;

  private:
    bool testMovement( double posX, double posY );

  protected:
    GameBoard * board;
    QTime lastPositionUpdate;
    QPointF position;
    QPointF direction;
    double velocity;
};

#endif

