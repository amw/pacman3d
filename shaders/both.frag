varying vec3 normal, lightDir, reflection, viewer;
varying float distance;

uniform sampler2D tex;

void main() {
  vec3 light, nNormal;
  vec4 texel;
  float NdotL, RdotV, attenuation;

  nNormal = normalize( normal );

  NdotL = max( dot( nNormal, normalize( lightDir ) ), 0.0 );
  light = gl_FrontLightProduct[ 0 ].ambient.rgb;

  if ( NdotL > 0.00001 ) {
    light += gl_FrontLightProduct[ 0 ].diffuse.rgb * NdotL;

    if ( gl_FrontMaterial.shininess > 0.00001 ) {
      RdotV = dot( normalize( reflection ), normalize( viewer ) );
      RdotV = max( RdotV, 0.0 );

      light +=
        gl_FrontLightProduct[ 0 ].specular.rgb *
        pow( RdotV, gl_FrontMaterial.shininess );
    }
  }

  if ( gl_LightSource[ 0 ].position.w > 0.999 ) {
    attenuation = 1.0 / (
      gl_LightSource[ 0 ].constantAttenuation +
      gl_LightSource[ 0 ].linearAttenuation * distance +
      gl_LightSource[ 0 ].quadraticAttenuation * distance * distance );

    light = attenuation * light;
  }

  light += gl_FrontLightModelProduct.sceneColor.rgb;
  texel = texture2D( tex,gl_TexCoord[ 0 ].st );
  gl_FragColor = vec4( texel.rgb * light, texel.a );
}

