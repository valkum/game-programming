#version 330 core

in vec3 aPosition;
in vec3 aNormal;
in vec3 aTexCoord;

out vec3 vTexCoord;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uViewProjectionMatrix;
uniform mat4 uMVP;
uniform mat4 uModelMatrix;

void main()
{
    vec4 pos = uMVP * vec4(aPosition, 1.0);
    gl_Position = pos.xyww; 
    vTexCoord = aPosition;
}  
