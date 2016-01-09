#version 330

in vec3 vTexCoord;
in vec3 vNormal;


// Ouput data
out vec4 oColor;

uniform sampler2D uTexture;

void main () {
  vec3 color = texture(uTexture, vTexCoord.xy).rgb;
  //color *= dot(normalize(vNormal),vec3(0,0,1));

  oColor = vec4(1.0, 1.0, 0.0, 1.0);
}
