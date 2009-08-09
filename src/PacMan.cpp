#include "PacMan.hpp"
#include <QDebug>


struct XYZ {
  double x;
  double y;
  double z;
};

PacMan::PacMan( GameBoard * board )
  : MovingObject( board )
{
}

PacMan::~PacMan() {
  glDeleteLists( this->sphereList, 1 );
  gluDeleteQuadric( this->sphereQuadric );
}

void PacMan::initializeGL( QGLWidget & target ) {
  QImage pacman( "textures/pacman.png" );
  this->pacmanTexture = target.bindTexture( pacman );

  this->sphereList = glGenLists( 1 );
  this->sphereQuadric = gluNewQuadric();
  gluQuadricTexture( this->sphereQuadric, true );

  glNewList( this->sphereList, GL_COMPILE );
  {
    glBindTexture( GL_TEXTURE_2D, this->pacmanTexture );
    glRotatef( 180, 1.0f, 0.0f, 0.0f );
    gluSphere( this->sphereQuadric, 0.4f, 360 / 5, 180 / 5 );
  }
  glEndList();
}

void PacMan::render( QGLWidget & ) {
  glPushMatrix();
  glTranslatef(
    this->position.x() + 0.5f,
    this->position.y() + 0.5f,
    0.5f
  );
  glCallList( this->sphereList );
  glPopMatrix();
}

