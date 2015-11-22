#version 330 

// Values that stay constant for the whole mesh.
// uniform mat4 MVP;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat3 uNormalMatrix;

in vec3 aPosition;
in vec3 aNormal;
in vec3 aTexCoord;

// Output data ; will be interpolated for each fragment.
out vec3 vNormal;
//out vec3 fragmentColor;

void main () {
	// gl_Position = MVP * vec4(vertexPosition_modelspace,1);
	// // Position of the vertex, in worldspace : M * position
	// Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;

	// // Vector that goes from the vertex to the camera, in camera space.
	// // In camera space, the camera is at the origin (0,0,0).
	// vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
	// EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
  // vNormal     = inverse(transpose(mat3(uViewMatrix))) * aNormal;
  // vTexCoord   = aTexCoord;
  // gl_Position = uProjectionMatrix * uViewMatrix * vec4(aPosition, 1.0);

  vNormal     = uNormalMatrix * aNormal;
  gl_Position = uProjectionMatrix * uViewMatrix * vec4(aPosition, 1.0f);
	// fragmentColor = vertexColor;
}