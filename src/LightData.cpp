#include "LightData.hpp"


void LightData::setAmbient( float r, float g, float b ) {
  this->ambient[ 0 ] = r;
  this->ambient[ 1 ] = g;
  this->ambient[ 2 ] = b;
}

void LightData::setSpecular( float r, float g, float b ) {
  this->specular[ 0 ] = r;
  this->specular[ 1 ] = g;
  this->specular[ 2 ] = b;
}

void LightData::setDiffuse( float r, float g, float b ) {
  this->diffuse[ 0 ] = r;
  this->diffuse[ 1 ] = g;
  this->diffuse[ 2 ] = b;
}


