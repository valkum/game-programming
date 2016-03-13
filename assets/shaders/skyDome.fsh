#version 330 core
in vec2 vTexCoord;

out vec4 oColor;

uniform sampler2D uTexture;
//uniform vec3 cameraCenter;

void main()
{   
    oColor = texture(uTexture, vTexCoord);
    //oColor = vec4(1.0, 0.0, 0.0, 1.0);
}