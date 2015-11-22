#version 330

in vec3 vNormal;
//in vec3 fragmentColor;
// Ouput data
out vec4 oColor;

void main () {
  // color = fragmentColor;
  // color *= dot(normalize(vNormal),vec3(0,0,1));
  oColor = vec4(normalize(vNormal), 1.0);
}