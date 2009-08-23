#include "Light.hpp"


Light::Light() {
  this->ambient[ 0 ] = 0.2f;
  this->ambient[ 1 ] = 0.2f;
  this->ambient[ 2 ] = 0.2f;
  this->ambient[ 3 ] = 1.0f;

  this->diffuse[ 0 ] = 0.8f;
  this->diffuse[ 1 ] = 0.8f;
  this->diffuse[ 2 ] = 0.8f;
  this->diffuse[ 3 ] = 1.0f;

  this->specular[ 0 ] = 0.0f;
  this->specular[ 1 ] = 0.0f;
  this->specular[ 2 ] = 0.0f;
  this->specular[ 3 ] = 1.0f;
}

void Light::updateGlState( GLenum light ) const {
  glLightfv( light, GL_AMBIENT, this->ambient );
  glLightfv( light, GL_DIFFUSE, this->diffuse );
  glLightfv( light, GL_SPECULAR, this->specular );
}

