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
										// Values that stay constant for the whole mesh.
										uniform mat4 MVP;
										void main () {
										  gl_Position = MVP * vec4(vertexPosition_modelspace,1);
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