#include "Material.hpp"


Material::Material() {
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

  this->emission[ 0 ] = 0.0f;
  this->emission[ 1 ] = 0.0f;
  this->emission[ 2 ] = 0.0f;
  this->emission[ 3 ] = 1.0f;

  this->shininess = 0.0f;
}

void Material::updateGlState( Face face ) const {
  glMaterialfv( face, GL_AMBIENT, this->ambient );
  glMaterialfv( face, GL_DIFFUSE, this->diffuse );
  glMaterialfv( face, GL_SPECULAR, this->specular );
  glMaterialfv( face, GL_EMISSION, this->emission );
  glMaterialf( face, GL_SHININESS, this->shininess );
}

void Material::setEmission( float r, float g, float b ) {
  this->emission[ 0 ] = r;
  this->emission[ 1 ] = g;
  this->emission[ 2 ] = b;
}

void Material::setShininess( float shininess ) {
  this->shininess = shininess;
}

