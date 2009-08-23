#ifndef LIGHTDATA_HPP
#define LIGHTDATA_HPP


class LightData {
  public:
    void setAmbient( float r, float g, float b );
    void setSpecular( float r, float g, float b );
    void setDiffuse( float r, float g, float b );

  protected:
    float ambient[ 4 ];
    float specular[ 4 ];
    float diffuse[ 4 ];
};

#endif

