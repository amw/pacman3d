varying vec3 normal, lightDir, halfVec;

void main() {
  normal = normalize( gl_NormalMatrix * gl_Normal );
  lightDir = normalize( gl_LightSource[ 0 ].position.xyz );
  halfVec = normalize( gl_LightSource[ 0 ].halfVector.xyz );

  gl_Position = ftransform();
  gl_TexCoord[ 0 ] = gl_MultiTexCoord0;
}

