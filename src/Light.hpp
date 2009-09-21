#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "LightData.hpp"
#include "gl.h"

class Light : public LightData {
  public:
    Light();
    void updateGlState( GLenum light ) const;
    void setAttenuation( float constant, float linear, float quadratic );
    void setPosition( float x, float y, float z );
    void setDirection( float x, float y, float z );

  private:
    float attenuation[ 3 ];
    float position[ 4 ];
};

#endif

