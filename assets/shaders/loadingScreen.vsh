#version 330 

in vec3 aPosition;
out float opacity;

uniform float uTime;
uniform mat4 uModelMatrix;


float cubicInOut(float t) {
  return t < 0.5
    ? 4.0 * t * t * t
    : 0.5 * pow(2.0 * t - 2.0, 3.0) + 1.0;
}
float cubicOut(float t) {
  float f = t - 1.0;
  return f * f * f + 1.0;
}
float quarticInOut(float t) {
  return t < 0.5
    ? +8.0 * pow(t, 4.0)
    : -8.0 * pow(t - 1.0, 4.0) + 1.0;
}

void main () {
  // opacity = 0.5 + cos( uTime/2 * 3.141592653589793 ) / 2;
  float x = uTime/3;
  opacity = 1 - quarticInOut(x);
  if(opacity<=0) opacity = 0;
  gl_Position = vec4(aPosition, 1.0);
}