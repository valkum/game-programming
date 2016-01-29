#version 330 

in vec3 aPosition;
out vec3 vColor;

uniform vec3 uColor;
uniform mat4 uModelMatrix;

void main () {
  vColor     = uColor;
  gl_Position = vec4(aPosition, 1.0);
}