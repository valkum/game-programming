#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "gamestate.h"


// @todo: Ist noch ein HACK, auf lange sich aus files laden.
#define GLSL(version, shader)  "#version " #version "\n" #shader

static const char* vertex_shader = GLSL(400, 
										layout(location = 0) in vec3 vertexPosition_modelspace;
										layout(location = 1) in vec3 vertexColor;
										// Output data ; will be interpolated for each fragment.
										out vec3 fragmentColor;
										out vec3 Position_worldspace;
										out vec3 EyeDirection_cameraspace;
										// Values that stay constant for the whole mesh.
										uniform mat4 MVP;
										uniform mat4 V;
										uniform mat4 M;
										void main () {
											gl_Position = MVP * vec4(vertexPosition_modelspace,1);
											// Position of the vertex, in worldspace : M * position
											Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;

											// Vector that goes from the vertex to the camera, in camera space.
											// In camera space, the camera is at the origin (0,0,0).
											vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
											EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
											fragmentColor = vertexColor;
										}
                                     	);
static const char* fragment_shader = GLSL(400,
										in vec3 fragmentColor;
										// Ouput data
										out vec3 color;
										void main () {
										  color = fragmentColor;
										}
										);

class IntroState : public CGameState 
{
public:
	void Init();
	void Draw(CGame* game, float* delta);

	static IntroState* Instance() {
		return &m_IntroState;
	}
protected:
	IntroState() {}
private:
	static IntroState m_IntroState;
};
#endif