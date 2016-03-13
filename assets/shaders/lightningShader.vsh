#version 330 

in vec3 aPosition;
in vec3 aNormal;


// Output data ; will be interpolated for each fragment.
//out vec3 vNormal;
out vec3 vColor;

struct Light {
  vec3 direction;
  float ambient;
  vec3 color;
  float diffuse;
  float specular;
};

uniform mat4 uMVP;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat3 uNormalMatrix;
uniform vec3 camera;
uniform Light uLight;
uniform vec3 uColor;


void main () {
  vec3 ambientColor = uLight.ambient * uLight.color;
  vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
  vec3 specularColor = vec3(0.0, 0.0, 0.0);


  vec3 direction = normalize(uLight.direction);

  gl_Position = uMVP * vec4(aPosition, 1.0f);
  //vNormal     = inverse(transpose(mat3(uViewMatrix))) * aNormal;

  //diffuse light
  diffuseColor += clamp(dot(normalize(aNormal), direction)
        *uLight.diffuse, 0.0, 1.0f);

  //specular
  vec3 cameraVector = normalize(camera - vec3(aPosition));
  specularColor += clamp(pow((dot((cameraVector+direction), normalize(aNormal))/
        (length(cameraVector+direction) * length(normalize(aNormal)))), 0.7f), 0.0, 1.0f)
        *uLight.specular;

  vec3 result = (ambientColor + diffuseColor + specularColor) * uColor;
  vColor = result;
}
