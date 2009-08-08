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
}

void PacMan::initializeGL( QGLWidget & target ) {
  QImage pacman( "textures/pacman.png" );
  this->pacmanTexture = target.bindTexture( pacman );

  this->sphereList = glGenLists( 1 );
  glNewList( this->sphereList, GL_COMPILE );
  {
    this->addSphere( 5, 5 );
  }
  glEndList();
}

void PacMan::render( QGLWidget & ) {
  glCallList( this->sphereList );
}

void PacMan::addSphere( int dTheta, int dPhi ) {
  glBindTexture( GL_TEXTURE_2D, this->pacmanTexture );
  glBegin( GL_QUADS );
  {
    for ( int theta = -90; theta <= 90 - dTheta; theta += dTheta ) {
      for ( int phi = 0; phi <= 360 - dPhi; phi += dPhi ) {
        this->addSphereVertex( theta         , phi );
        this->addSphereVertex( theta + dTheta, phi );
        this->addSphereVertex( theta + dTheta, phi + dPhi );
        this->addSphereVertex( theta         , phi + dPhi );
      }
    }
  }
  glEnd();
}

void PacMan::addSphereVertex( int theta, int phi ) {
  double x, y, z, s, t;

  x = cos( theta * DEG_TO_RAD ) * cos( phi * DEG_TO_RAD );
  y = cos( theta * DEG_TO_RAD ) * sin( phi * DEG_TO_RAD );
  z = sin( theta * DEG_TO_RAD );
  s = ( (double) phi ) / 360.0f;
  t = ( (double) theta + 90.0f ) / 180.0f;

  glTexCoord2f( s, t );
  glVertex3d( x , y, z );
}

