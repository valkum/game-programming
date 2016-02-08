#version 330

in vec3 aPosition;
in vec3 aNormal;

uniform mat4 uMVP;

void main() {
    gl_Position = uMVP * vec4(aPosition, 1.0);
}
