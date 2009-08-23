#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "LightData.hpp"
#include <QtOpenGL>

class Light : public LightData {
  public:
    Light();
    void updateGlState( GLenum light ) const;
};

#endif

