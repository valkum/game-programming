#version 330 core
in vec2 vTexCoords;
in vec4 vParticleColor;

out vec4 oColor;

uniform sampler2D uTexture;

void main()
{ 
    vec4 color = texture(uTexture, vTexCoords);
    oColor = color * vParticleColor;
    //oColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * vParticleColor;
}  