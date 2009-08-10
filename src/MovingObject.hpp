#ifndef MOVINGOBJECT_HPP
#define MOVINGOBJECT_HPP

#include "GameBoard.hpp"
#include <QPointF>
#include <QPoint>
#include <QObject>
#include <QTime>

#define MINIMUM_VELOCITY 0.05f
#define CENTER_ATTRACTION 0.15f

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
    double alignToCenter( double position );
    virtual bool canAccess( int x, int y ) = 0;
    virtual bool atBlockCenter() = 0;

  private:
    bool testMovement( double posX, double posY );
    void testIfAtBlockCenter();

  protected:
    GameBoard * board;
    QTime lastPositionUpdate;
    QPointF position;
    QPointF direction;
    double velocity;

  private:
    QPoint lastCenter;
};

#endif

