#include "Light.hpp"


Light::Light() {
  this->ambient[ 0 ] = 0.0f;
  this->ambient[ 1 ] = 0.0f;
  this->ambient[ 2 ] = 0.0f;
  this->ambient[ 3 ] = 1.0f;

  this->diffuse[ 0 ] = 1.0f;
  this->diffuse[ 1 ] = 1.0f;
  this->diffuse[ 2 ] = 1.0f;
  this->diffuse[ 3 ] = 1.0f;

  this->specular[ 0 ] = 1.0f;
  this->specular[ 1 ] = 1.0f;
  this->specular[ 2 ] = 1.0f;
  this->specular[ 3 ] = 1.0f;

  this->attenuation[ 0 ] = 1.0f;
  this->attenuation[ 1 ] = 0.0f;
  this->attenuation[ 2 ] = 0.0f;
}

void Light::updateGlState( GLenum light ) const {
  glLightfv( light, GL_POSITION, this->position );
  glLightfv( light, GL_AMBIENT, this->ambient );
  glLightfv( light, GL_DIFFUSE, this->diffuse );
  glLightfv( light, GL_SPECULAR, this->specular );
  glLightf( light, GL_CONSTANT_ATTENUATION, this->attenuation[ 0 ] );
  glLightf( light, GL_LINEAR_ATTENUATION, this->attenuation[ 1 ] );
  glLightf( light, GL_QUADRATIC_ATTENUATION, this->attenuation[ 2 ] );
}

void Light::setAttenuation( float constant, float linear, float quadratic ) {
  this->attenuation[ 0 ] = constant;
  this->attenuation[ 1 ] = linear;
  this->attenuation[ 2 ] = quadratic;
}

void Light::setPosition( float x, float y, float z ) {
  this->position[ 0 ] = x;
  this->position[ 1 ] = y;
  this->position[ 2 ] = z;
  this->position[ 3 ] = 1.0f;
}

void Light::setDirection( float x, float y, float z ) {
  this->position[ 0 ] = x;
  this->position[ 1 ] = y;
  this->position[ 2 ] = z;
  this->position[ 3 ] = 0.0f;
}

