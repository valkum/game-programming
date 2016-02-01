#version 330 core
in vec2 vTexCoords;
in vec4 vParticleColor;

out vec4 oColor;

uniform sampler2D uTexture;

void main()
{ 
    vec4 color = texture(uTexture, vTexCoords);
    //TODO: implement soft particles (ie whenn colliding with opaque vertices)
    oColor = color * vParticleColor;
}  