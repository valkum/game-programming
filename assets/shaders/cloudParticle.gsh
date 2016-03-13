#version 330
layout(points) in;
layout(triangle_strip, max_vertices=8) out;

// in Camera {
//   vec3 up;
//   vec3 right;
// } camera[];
in vec4 vRGBA[];
 
uniform mat4 uProjectionMatrix;
 
uniform vec2 uSize;
 
out vec2 vTexCoords;
// out vec2 vPosition;
out float gDepth;
out vec4 gRGBA;
 
void main()
{
  gRGBA = vRGBA[0];
  // vec3 right = camera[0].right;
  // vec3 up = camera[0].up;
  vec4 P = gl_in[0].gl_Position;

  vec2 va = P.xy + vec2(-0.5, -0.5) * uSize;
  gl_Position = uProjectionMatrix * vec4(va, P.z, 1);
  vTexCoords = vec2(0.0, 0.0);
  gDepth = P.z;
  // vPosition = va;
  EmitVertex();

  vec2 vb = P.xy + vec2(-0.5, 0.5)  * uSize;
  gl_Position = uProjectionMatrix * vec4(vb, P.z, 1);
  vTexCoords = vec2(0.0, 1.0);
  gDepth = P.z;
  // vPosition = vb;
  EmitVertex();

  vec2 vc = P.xy + vec2(0.5, -0.5) * uSize;
  gl_Position = uProjectionMatrix * vec4(vc, P.z, 1);
  vTexCoords = vec2(1.0, 0.0);
  gDepth = P.z;
  // vPosition = vc;
  EmitVertex();

  vec2 vd = P.xy + vec2(0.5, 0.5) * uSize;
  gl_Position = uProjectionMatrix * vec4(vd, P.z, 1);
  vTexCoords = vec2(1.0, 1.0);
  gDepth = P.z;
  // vPosition = vd;
  EmitVertex();

  EndPrimitive();
}