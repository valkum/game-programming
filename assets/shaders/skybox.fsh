#version 330 core
in vec3 vTexCoord;

out vec4 oColor;

uniform samplerCube uTexture;
//uniform vec3 cameraCenter;

void main()
{   
    oColor = texture(uTexture, vTexCoord);
}