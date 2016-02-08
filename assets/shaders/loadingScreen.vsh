#version 330 

in vec3 aPosition;

uniform mat4 uModelMatrix;


void main () {
  gl_Position = vec4(aPosition, 1.0);
}