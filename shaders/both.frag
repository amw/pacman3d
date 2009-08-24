varying vec3 normal, position;

uniform int lightsCount;
uniform sampler2D tex;


#define getLightComponent( i )                                                 \
                                                                               \
if ( lightsCount > i ) {                                                       \
  vec3 light = gl_FrontLightProduct[ i ].ambient.rgb;                          \
  vec3 lightDir;                                                               \
  float distance;                                                              \
                                                                               \
  if ( gl_LightSource[ i ].position.w > 0.999 ) {                              \
    lightDir = vec3( gl_LightSource[ i ].position.xyz - position );            \
    distance = length( lightDir );                                             \
    lightDir = normalize( lightDir );                                          \
  }                                                                            \
  else {                                                                       \
    lightDir = normalize( gl_LightSource[ i ].position.xyz );                  \
  }                                                                            \
                                                                               \
  float NdotL = max( dot( nNormal, lightDir ), 0.0 );                          \
  if ( NdotL > 0.00001 ) {                                                     \
    light += gl_FrontLightProduct[ i ].diffuse.rgb * NdotL;                    \
                                                                               \
    if ( gl_FrontMaterial.shininess > 0.00001 ) {                              \
      vec3 reflection = normalize( reflect( lightDir, nNormal ) );             \
      float RdotV = max( dot( reflection, normalize( position ) ), 0.0 );      \
                                                                               \
      light +=                                                                 \
        gl_FrontLightProduct[ i ].specular.rgb *                               \
        pow( RdotV, gl_FrontMaterial.shininess );                              \
    }                                                                          \
  }                                                                            \
                                                                               \
  if ( gl_LightSource[ i ].position.w > 0.999 ) {                              \
    float attenuation = 1.0 / (                                                \
      gl_LightSource[ i ].constantAttenuation +                                \
      gl_LightSource[ i ].linearAttenuation * distance +                       \
      gl_LightSource[ i ].quadraticAttenuation * distance * distance           \
    );                                                                         \
                                                                               \
    light = attenuation * light;                                               \
  }                                                                            \
                                                                               \
  sumOfLights += light;                                                        \
}

void main() {
  vec3 nNormal = normalize( normal );

  vec3 sumOfLights = gl_FrontLightModelProduct.sceneColor.rgb;

  getLightComponent( 0 )
  getLightComponent( 1 )
  getLightComponent( 2 )
  getLightComponent( 3 )
  getLightComponent( 4 )
  getLightComponent( 5 )
  getLightComponent( 6 )
  getLightComponent( 7 )

  vec4 texel = texture2D( tex,gl_TexCoord[ 0 ].st );
  gl_FragColor = vec4( texel.rgb * sumOfLights, texel.a );
}

