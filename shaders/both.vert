varying vec3 normal, lightDir, reflection, viewer;
varying float distance;

void main() {
  normal = normalize( gl_NormalMatrix * gl_Normal );
  gl_Position = ftransform();
  gl_TexCoord[ 0 ] = gl_MultiTexCoord0;

  vec4 vertexPosition = gl_ModelViewMatrix * gl_Vertex;

  if ( gl_LightSource[ 0 ].position.w < 0.001 ) {
    // directional light

    lightDir = normalize( gl_LightSource[ 0 ].position.xyz );
    distance = 0.0;
  }
  else {
    // positional light

    lightDir = vec3( gl_LightSource[ 0 ].position - vertexPosition );
    distance = length( lightDir );
    lightDir = normalize( lightDir );
  }

  reflection = normalize( reflect( lightDir, normal ) );
  viewer = normalize( vertexPosition.xyz );
}

