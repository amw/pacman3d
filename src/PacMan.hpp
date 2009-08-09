#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "GameBoard.hpp"
#include "MovingObject.hpp"
#include <QGLWidget>

class PacMan : public MovingObject {
  Q_OBJECT

  public:
    PacMan( GameBoard * board );
    ~PacMan();
    void initializeGL( QGLWidget & target );
    void render( QGLWidget & target );

  private:
    void addSphere( int dTheta, int dPhi );
    void addSphereVertex( int theta, int phi );

  private:
    GLUquadric* sphereQuadric;
    GLuint sphereList;
    GLuint pacmanTexture;
};

#endif

