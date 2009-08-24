varying vec3 normal, position;

void main() {
  gl_Position = ftransform();
  gl_TexCoord[ 0 ] = gl_MultiTexCoord0;

  normal = normalize( gl_NormalMatrix * gl_Normal );
  position = vec3( gl_ModelViewMatrix * gl_Vertex );
}

