#version 330
in float opacity;
out vec4 oColor;

uniform vec3 uColor;

void main () {
  oColor = vec4(uColor, opacity);
}