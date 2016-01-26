#version 330
layout(points) in;
layout(triangle_strip, max_vertices=8) out;

// in Camera {
//   vec3 up;
//   vec3 right;
// } camera[];
 
uniform mat4 uProjectionMatrix;
 
uniform vec2 uSize;
 
out vec2 vTexCoords;
 
void main()
{
  // vec3 right = camera[0].right;
  // vec3 up = camera[0].up;
  vec4 P = gl_in[0].gl_Position;

  vec2 va = P.xy + vec2(-0.5, -0.5) * uSize;
  gl_Position = uProjectionMatrix * vec4(va, P.z, 1);
  vTexCoords = vec2(0.0, 0.0);
  EmitVertex();

  vec2 vb = P.xy + vec2(-0.5, 0.5)  * uSize;
  gl_Position = uProjectionMatrix * vec4(vb, P.z, 1);
  vTexCoords = vec2(0.0, 1.0);
  EmitVertex();

  vec2 vc = P.xy + vec2(0.5, -0.5) * uSize;
  gl_Position = uProjectionMatrix * vec4(vc, P.z, 1);
  vTexCoords = vec2(1.0, 0.0);
  EmitVertex();

  vec2 vd = P.xy + vec2(0.5, 0.5) * uSize;
  gl_Position = uProjectionMatrix * vec4(vd, P.z, 1);
  vTexCoords = vec2(1.0, 1.0);
  EmitVertex();

  EndPrimitive();
}