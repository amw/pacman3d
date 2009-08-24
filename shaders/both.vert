varying vec3 normal, halfVec, lightDir;
varying vec3 globalAmbient, ambient, diffuse, specular;
varying float distance;

void main() {
  normal = normalize( gl_NormalMatrix * gl_Normal );
  halfVec = normalize( gl_LightSource[ 0 ].halfVector.xyz );

  if ( gl_LightSource[ 0 ].position.w < 0.001 ) {
    // directional light

    lightDir = normalize( gl_LightSource[ 0 ].position.xyz );
    distance = 0.0;
  }
  else {
    // positional light

    vec4 vertexPosition = gl_ModelViewMatrix * gl_Vertex;
    vec3 lightToVertex = vec3( gl_LightSource[ 0 ].position - vertexPosition );

    lightDir = normalize( lightToVertex );
    distance = length( lightToVertex );
  }

  globalAmbient = gl_FrontMaterial.ambient.rgb * gl_LightModel.ambient.rgb;
  ambient = gl_FrontMaterial.ambient.rgb * gl_LightSource[ 0 ].ambient.rgb;
  diffuse = gl_FrontMaterial.diffuse.rgb * gl_LightSource[ 0 ].diffuse.rgb;
  specular = gl_FrontMaterial.specular.rgb * gl_LightSource[ 0 ].specular.rgb;

  gl_Position = ftransform();
  gl_TexCoord[ 0 ] = gl_MultiTexCoord0;
}

