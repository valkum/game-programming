#version 330 core

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelViewProjectionMatrix;
uniform mat4 uModelMatrix;
uniform vec3 uCameraCenter;

in vec3 aPosition;
in vec3 aNormal;
in vec3 aTexCoord;

out vec3 vTexCoord;

void main()
{
    vTexCoord = aTexCoord;
    gl_Position =   (uModelViewProjectionMatrix * vec4(aPosition, 1.0)).xyww;  
    
}  