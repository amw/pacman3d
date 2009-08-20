
varying vec4 position;

void main() {
  gl_TexCoord[ 0 ] = gl_MultiTexCoord0;
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
  position = gl_Position;
}

