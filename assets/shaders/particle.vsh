#version 330 core
in vec3 aPosition;
in vec2 aTexCoord;

out vec2 vTexCoords;
out vec4 vParticleColor;

uniform mat4 uMVP;
uniform vec3 uOffset;
uniform vec4 uColor;

void main()
{
    vTexCoords = aTexCoord;
    vParticleColor = uColor;
    gl_Position = uMVP * vec4((aPosition) + uOffset, 1.0);
}