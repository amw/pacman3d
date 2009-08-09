#ifndef MOVINGOBJECT_HPP
#define MOVINGOBJECT_HPP

#include "GameBoard.hpp"
#include <QPointF>
#include <QObject>

class MovingObject : public QObject {
  Q_OBJECT

  public:
    MovingObject( GameBoard * board );
    const QPointF & getPosition() const;
    const QPointF & getDirection() const;
    double getVelocity() const;
    void setPosition( QPointF position );

  protected:
    GameBoard * board;
    QPointF position;
    QPointF direction;
    double velocity;
};

#endif

