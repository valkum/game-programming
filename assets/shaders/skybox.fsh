#version 330 core
in vec3 vTexCoord;

out vec4 oColor;

uniform samplerCube uTexture;
//uniform vec3 cameraCenter;

void main()
{   
    //float val = sin(vTexCoord.x);

    oColor = texture(uTexture, -vTexCoord);
    //oColor = vec4(vTexCoord.x, vTexCoord.y, vTexCoord.z, 1.0);

}