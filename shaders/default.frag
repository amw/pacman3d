
uniform int  ghostsCount;
uniform int  dotsCount;
uniform sampler2D tex;

varying vec4 position;

void main() {
  vec4 textureColor = texture2D( tex, gl_TexCoord[ 0 ].st );
  vec4 lightEffect = textureColor * gl_LightModel.ambient;

/*
  for ( int i = 0; i < lightsCount; ++i ) {
    vec4 vector = vec4( lightsPosition[ i ], 0.0 );
    vector = vector * gl_ProjectionMatrix;
    vector = position - vector;
    float distance = length( vector );
    if ( distance < 30.0 ) {
      lightEffect = vec3( 1.0, 0.0, 0.0 );
    }

    if ( i > 60 ) {
      break;
    }
  }
*/
  lightEffect.r = 1.0;

  gl_FragColor = lightEffect;
}

