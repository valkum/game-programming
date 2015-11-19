#version 330

in vec3 fragmentColor;
// Ouput data
out vec3 color;
void main () {
  color = fragmentColor;
}