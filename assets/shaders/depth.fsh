#version 330

uniform float zFar;
uniform float zNear;
//out vec4 oColor;
void main() {
    //oColor = vec4(1.0);
    gl_FragDepth = gl_FragCoord.z;
}
