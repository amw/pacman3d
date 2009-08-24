varying vec3 normal, lightDir, reflection, viewer;
varying vec3 globalAmbient, ambient, diffuse, specular;
varying float distance;
uniform sampler2D tex;

void main() {
  vec3 light, nNormal;
  vec4 texel;
  float NdotL, RdotV, attenuation;

  nNormal = normalize( normal );
  NdotL = max( dot( nNormal, normalize( lightDir ) ), 0.0 );

  light = globalAmbient;

  if ( gl_LightSource[ 0 ].position.w < 0.001 ) {
    // directional light

    light += ambient;

    if ( NdotL > 0.00001 ) {
      light += diffuse * NdotL;

      if ( gl_FrontMaterial.shininess > 0.00001 ) {
        RdotV = dot( normalize( reflection ), normalize( viewer ) );
        RdotV = max( RdotV, 0.0 );

        light +=
          diffuse * NdotL +
          specular * pow( RdotV, gl_FrontMaterial.shininess );
      }
    }
  }
  else {
    // positional light

    attenuation = 1.0 / (
      gl_LightSource[ 0 ].constantAttenuation +
      gl_LightSource[ 0 ].linearAttenuation * distance +
      gl_LightSource[ 0 ].quadraticAttenuation * distance * distance );

    if ( NdotL > 0.00001 ) {
      if ( gl_FrontMaterial.shininess > 0.00001 ) {
        RdotV = dot( normalize( reflection ), normalize( viewer ) );
        RdotV = max( RdotV, 0.0 );

        light += attenuation * (
          ambient +
          diffuse * NdotL +
          specular * pow( RdotV, gl_FrontMaterial.shininess )
        );
      }
      else {
        light += attenuation * (
          ambient +
          diffuse * NdotL
        );
      }
    }
    else {
      light += attenuation * ambient;
    }
  }

  texel = texture2D( tex,gl_TexCoord[ 0 ].st );

  gl_FragColor = vec4( texel.rgb * light, texel.a );
}

