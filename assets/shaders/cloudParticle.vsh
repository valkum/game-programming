#version 330 core
in vec3 aPosition;

// out Camera {
//   vec3 up;
//   vec3 right;
// } camera;

// uniform mat4 uMVP;
// uniform mat4 uScale;
// uniform vec3 uOffset;
uniform vec4 uColor;
// uniform vec3 uCameraRight_worldspace;
// uniform vec3 uCameraUp_worldspace;
uniform mat4 uProjectionMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uMVP;

void main()
{
    // camera.up    = vec3(uModelViewMatrix[0][0],
    //                     uModelViewMatrix[1][0],
    //                     uModelViewMatrix[2][0]);

    // camera.right = vec3(uModelViewMatrix[0][1],
    //                     uModelViewMatrix[1][1],
    //                     uModelViewMatrix[2][1]);
    //gl_PointSize = 10;
    gl_Position = uModelViewMatrix * vec4(aPosition, 1.0);
}