#version 330

in vec3 aPosition; 
in vec3 aColor;
in vec3 aNormal;

uniform mat4 uMVP;



out vec3 vColor;

void main ()
{
    vColor = aColor;
    gl_Position = uMVP * vec4(aPosition, 1.0);
}