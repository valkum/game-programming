#version 330
//#pragma ACGLimport "classicnoise2d.glsl"
//#pragma ACGLimport "noise2d.glsl"
#pragma ACGLimport "noise3d.glsl"
in vec2 vTexCoords;
in vec2 vPosition;

out vec4 oColor;

uniform sampler2D uTexture;
uniform vec4 uColor;
uniform float uTime;
void main()
{ 
    vec4 color = texture(uTexture, vTexCoords);
    // vec4 color = vec4(cnoise(vPosition+17.0), cnoise(vPosition), cnoise(vPosition+24.0), 1.0);
    oColor = color;
    // vec3 texCoords = vec3(vTexCoords, 100);
    // // Perturb the texcoords with three components of noise
    // vec3 uvw = texCoords + 0.1*vec3(snoise(texCoords + vec3(0.0, 0.0, uTime)),
    // snoise(texCoords + vec3(43.0, 17.0, uTime)),
    // snoise(texCoords + vec3(-17.0, -43.0, uTime)));
    // // Six components of noise in a fractal sum
    // float n = snoise(uvw - vec3(0.0, 0.0, uTime));
    // n += 0.5 * snoise(uvw * 2.0 - vec3(0.0, 0.0, uTime*1.4)); 
    // n += 0.25 * snoise(uvw * 4.0 - vec3(0.0, 0.0, uTime*2.0)); 
    // n += 0.125 * snoise(uvw * 8.0 - vec3(0.0, 0.0, uTime*2.8)); 
    // n += 0.0625 * snoise(uvw * 16.0 - vec3(0.0, 0.0, uTime*4.0)); 
    // n += 0.03125 * snoise(uvw * 32.0 - vec3(0.0, 0.0, uTime*5.6)); 
    // n = n * 0.7;
    // oColor = vec4(0.5 + (0.5 * vec3(n, n, n)), n);
}  