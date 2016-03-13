#version 330
in vec3 vColor;

// Ouput data
out vec4 oColor;


void main () {
  oColor = vec4(vColor, 1.0);
}
