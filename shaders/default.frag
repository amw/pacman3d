varying vec3 normal, lightDir, halfVec;
uniform sampler2D tex;

void main() {
  vec3 light, diffuse, ambient, globalAmbient, nNormal;
  vec3 specular = vec3( 0.0, 0.0, 0.0 );
  vec4 texel;
  float NdotL, NdotHV;

  nNormal = normalize( normal );

  NdotL = max( dot( nNormal, normalize( lightDir ) ), 0.0 );

  ambient =
    gl_FrontMaterial.ambient.rgb *
    gl_LightSource[ 0 ].ambient.rgb;

  globalAmbient =
    gl_FrontMaterial.ambient.rgb *
    gl_LightModel.ambient.rgb;

  diffuse =
    gl_FrontMaterial.diffuse.rgb *
    gl_LightSource[ 0 ].diffuse.rgb *
    NdotL;

  if ( NdotL > 0.0001 ) {
    NdotHV = max( dot( nNormal, normalize( halfVec.xyz ) ), 0.0 );

    specular =
      gl_FrontMaterial.specular.rgb *
      gl_LightSource[ 0 ].specular.rgb *
      pow( NdotHV, gl_FrontMaterial.shininess );
  }


  light = globalAmbient + ambient + diffuse + specular;

  texel = texture2D( tex,gl_TexCoord[ 0 ].st );

  gl_FragColor = vec4( texel.rgb * light, texel.a );
}

