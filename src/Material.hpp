#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "LightData.hpp"
#include "gl.h"

class Material : public LightData {
  public:
    enum Face {
      Front = GL_FRONT,
      Back = GL_BACK,
      FrontAndBack = GL_FRONT_AND_BACK
    };

  public:
    Material();
    void updateGlState( Face face ) const;
    void setEmission( float r, float g, float b );
    void setShininess( float shininess );

  private:
    float emission[ 4 ];
    float shininess;
};

#endif

