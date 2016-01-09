#version 330 

in vec3 aPosition;
in vec3 aTexCoord;
in vec3 aNormal;


// Output data ; will be interpolated for each fragment.
//out vec3 vNormal;
out vec3 vTexCoord;

uniform mat4 uMVP;
uniform mat4 uViewMatrix;
uniform mat3 uNormalMatrix;

void main () {
  //vNormal     = inverse(transpose(mat3(uViewMatrix))) * aNormal;
  vTexCoord     = aTexCoord;
  gl_Position = uMVP * vec4(aPosition, 1.0);
}