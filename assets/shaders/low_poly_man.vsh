#version 330 core
in vec3 aPosition;

out vec4 vertexColor;

uniform mat4 uMVP;

void main()
{
    gl_Position = uMVP * vec4(aPosition, 1.0);
    vertexColor = vec4(0.75f, 0.75f, 0.75f, 1.0f);
}
