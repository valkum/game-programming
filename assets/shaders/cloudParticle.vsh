#version 330 core
in vec3 aPosition;
in vec2 aTexCoord;

out vec2 vTexCoords;
out vec4 vParticleColor;

uniform mat4 uMVP;
uniform vec3 uOffset;
uniform vec4 uColor;
uniform vec3 uCameraRight_worldspace;
uniform vec3 uCameraUp_worldspace;

void main()
{
    vTexCoords = aTexCoord;
    vParticleColor = uColor;

    vec3 vertexPosition_worldspace = 
    uOffset
    + uCameraRight_worldspace * aPosition.x
    + uCameraUp_worldspace * aPosition.y;

    gl_Position = uMVP * vec4(vertexPosition_worldspace, 1.0);
}