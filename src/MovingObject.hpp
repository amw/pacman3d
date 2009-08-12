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
    const QPoint & getDirection() const;
    double getVelocity() const;
    void setPosition( QPointF position );
    void setDirection( QPoint direction, int timeStep );
    void setVelocity( double velocity, int timeStep );
    void updatePosition( int timeStep );

  protected:
    double alignToCenter( double position );
    virtual bool canAccess( int x, int y ) = 0;
    virtual void atBlockCenter() = 0;

  private:
    bool testMovement( double posX, double posY );
    void testIfAtBlockCenter();

  protected:
    GameBoard * board;
    QPointF position;
    QPoint direction;
    double velocity;
};

#endif

