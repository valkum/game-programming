#version 150

uniform mat4 uMVP;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat3 uNormalMatrix;

in vec3 aPosition;
in vec3 aNormal;

out Vertex
{
  vec3 normal;
  vec3 color;
} vertex;

void main() {
  vertex.normal = aNormal;
  vertex.color = vec3(1.0, 1.0, 0.0);
  gl_Position = vec4(aPosition, 1.0);
}