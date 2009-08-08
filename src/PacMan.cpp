#include "PacMan.hpp"
#include <math.h>

#define DEG_TO_RAD 0.0174532925f


struct XYZ {
  double x;
  double y;
  double z;
};

PacMan::PacMan() {
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
    gluSphere( this->sphereQuadric, 0.4f, 360 / 30, 180 / 5 );
  }
  glEndList();
}

void PacMan::render( QGLWidget & ) {
  glCallList( this->sphereList );
}

