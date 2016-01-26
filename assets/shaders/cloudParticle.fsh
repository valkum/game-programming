#version 330
in vec2 vTexCoords;

out vec4 oColor;

uniform sampler2D uTexture;
uniform vec4 uColor;

void main()
{ 
    vec4 color = texture(uTexture, vTexCoords);
    oColor = color;
    //oColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}  