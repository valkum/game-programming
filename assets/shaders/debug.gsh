#version 150
layout(triangles) in;
layout(line_strip, max_vertices=8) out;
 
uniform mat4 uMVP;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat3 uNormalMatrix;
 
in Vertex
{
  vec3 normal;
  vec3 color;
} vertex[];
 
out vec3 vColor;
 
void main()
{
  int i;
  
  //------ 3 lines for the 3 vertex normals
  //
  // for(i=0; i<gl_in.length(); i++)
  // {
  //   vec3 P = gl_in[i].gl_Position.xyz;
  //   vec3 N = vertex[i].normal;
    
  //   gl_Position = uMVP * vec4(P, 1.0);
  //   vColor = vertex[i].color;
  //   EmitVertex();
    
  //   gl_Position = uMVP * vec4(P + N * 1.0, 1.0);
  //   vColor = vertex[i].color;
  //   EmitVertex();
    
  //   EndPrimitive();
  // }
  
 
  //------ One line for the face normal
  //
  vec3 P0 = gl_in[0].gl_Position.xyz;
  vec3 P1 = gl_in[1].gl_Position.xyz;
  vec3 P2 = gl_in[2].gl_Position.xyz;
  
  // vec3 V0 = P0 - P1;
  // vec3 V1 = P2 - P1;
  
  // vec3 N = cross(V1, V0);
  // N = normalize(N);
  
  // Center of the triangle
  vec3 P = (P0+P1+P2) / 3.0;
  
  gl_Position = uMVP * vec4(P, 1.0);
  vColor = vec3(1, 0, 0);
  EmitVertex();
  
  gl_Position = uMVP * vec4(P + vertex[0].normal * 1.0, 1.0);
  vColor = vec3(1, 0, 0);
  EmitVertex();
  EndPrimitive();
}