#ifndef GHOST_HPP
#define GHOST_HPP

#include "GameBoard.hpp"
#include "MovingObject.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "gl.h"
#include <QGLWidget>

class Ghost : public MovingObject {
  Q_OBJECT

  public:
    Ghost( GameBoard * board, double r, double g, double b );
    ~Ghost();
    void initializeGL( QGLWidget & target );
    void render( QGLWidget & target );
    void updateGlLight( GLenum light );
    QPoint getNewDirection();

  protected:
    virtual bool canAccess( int x, int y );
    virtual bool crossingBlockCenter( int timeStep, QPointF newPosition );

  private:
    void addSphere( int dTheta, int dPhi );
    void addSphereVertex( int theta, int phi );

  private:
    Light light;
    Material material;
    GLUquadric* sphereQuadric;
    GLuint sphereList;
    GLuint ghostTexture;
};

#endif

