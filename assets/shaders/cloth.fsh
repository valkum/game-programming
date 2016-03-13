#version 330

in vec3 vColor;

out vec4 oColor;
 
void main()
{
  oColor = vec4(vColor, 1.0);
  //oColor = vec4(1.0f,0.0f,0.0f,1.0f);// * vec4(vColor, 1.0f);
 
}
