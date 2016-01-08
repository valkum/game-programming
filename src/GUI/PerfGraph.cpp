#include "GUI/PerfGraph.hh"
#include <ACGL/OpenGL/GL.hh>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef NANOVG_GLEW
#  include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "nanovg.h"

#include <iconv.h>

// timer query support
#ifndef GL_ARB_timer_query
#define GL_TIME_ELAPSED                   0x88BF
//typedef void (APIENTRY *pfnGLGETQUERYOBJECTUI64V)(GLuint id, GLenum pname, GLuint64* params);
//pfnGLGETQUERYOBJECTUI64V glGetQueryObjectui64v = 0;
#endif

/*
void PerfGraph::initGPUTimer(GPUtimer* timer)
{
	memset(timer, 0, sizeof(*timer));
}

void PerfGraph::startGPUTimer(GPUtimer* timer)
{
	if (!timer->supported)
		return;
	glBeginQuery(GL_TIME_ELAPSED, timer->queries[timer->cur % GPU_QUERY_COUNT] );
	timer->cur++;
}

int PerfGraph::stopGPUTimer(GPUtimer* timer, float* times, int maxTimes)
{
	NVG_NOTUSED(times);
	NVG_NOTUSED(maxTimes);
	GLint available = 1;
	int n = 0;
	if (!timer->supported)
		return 0;

	glEndQuery(GL_TIME_ELAPSED);
	while (available && timer->ret <= timer->cur) {
		// check for results if there are any
		glGetQueryObjectiv(timer->queries[timer->ret % GPU_QUERY_COUNT], GL_QUERY_RESULT_AVAILABLE, &available);
		if (available) {
		}
	}
	return n;
}
*/


PerfGraph::PerfGraph(GUIObject *parent, int style, std::string name)
    : GUIObject(parent), mStyle(style), mCaption(name), mBackgroundColor(Color(0,0)), mForegroundColor(Color(100,125)), mTextColor(Color(0,0)){
	    prevt = glfwGetTime();
	    for (int i = 0; i < GRAPH_HISTORY_COUNT; ++i)
	    {
	    	values[i] = 0.0f;
	    }
    }
	//memset(fps, 0, sizeof(PerfGraph));
    //initGPUTimer(gputimer);
	//this->style = style;
    //this->name = name;
	//name[sizeof(name)-1] = '\0';
//}

//PerfGraph::~PerfGraph()
//{
//    stopGPUTimer(gputimer);
//}

void PerfGraph::updateGraph(float frameTime)
{
	head = (head+1) % GRAPH_HISTORY_COUNT;
	values[head] = frameTime;
}

float PerfGraph::getGraphAverage()
{
	int i;
	float avg = 0;
	for (i = 0; i < GRAPH_HISTORY_COUNT; i++) {
		avg += values[i];
	}
	return avg / (float)GRAPH_HISTORY_COUNT;
}

void PerfGraph::draw(NVGcontext* vg)
{
	int i;
	float avg;
	char str[64];

	avg = getGraphAverage();


	nvgBeginPath(vg);
	nvgRect(vg, mPosition.x, mPosition.y, mSize.x, mSize.y);
	nvgFillColor(vg, mBackgroundColor);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgMoveTo(vg, mPosition.x, mPosition.y+mSize.y);
	if (mStyle == GRAPH_RENDER_FPS) {
		for (i = 0; i < GRAPH_HISTORY_COUNT; i++) {
			float v = 1.0f / (0.00001f + values[(head+i) % GRAPH_HISTORY_COUNT]);
			float vx, vy;
			if (v > 80.0f) v = 80.0f;
			vx = mPosition.x + ((float)i/(GRAPH_HISTORY_COUNT-1)) * mSize.x;
			vy = mPosition.y +mSize.y - ((v / 80.0f) * mSize.y);
			nvgLineTo(vg, vx, vy);
		}
	} else if (mStyle == GRAPH_RENDER_PERCENT) {
		for (i = 0; i < GRAPH_HISTORY_COUNT; i++) {
			float v = values[(head+i) % GRAPH_HISTORY_COUNT] * 1.0f;
			float vx, vy;
			if (v > 100.0f) v = 100.0f;
			vx = mPosition.x + ((float)i/(GRAPH_HISTORY_COUNT-1)) * mSize.x;
			vy = mPosition.y +mSize.y - ((v / 100.0f) * mSize.y);
			nvgLineTo(vg, vx, vy);
		}
	} else {
		for (i = 0; i < GRAPH_HISTORY_COUNT; i++) {
			float v = values[(head+i) % GRAPH_HISTORY_COUNT] * 1000.0f;
			float vx, vy;
			if (v > 20.0f) v = 20.0f;
			vx = mPosition.x + ((float)i/(GRAPH_HISTORY_COUNT-1)) * mSize.x;
			vy = mPosition.y +mSize.y - ((v / 20.0f) * mSize.y);
			nvgLineTo(vg, vx, vy);
		}
	}
	nvgLineTo(vg, mPosition.x+mSize.x, mPosition.y+mSize.y);
	nvgFillColor(vg, mForegroundColor);
	nvgFill(vg);

	nvgFontFace(vg, "sans");

	
		nvgFontSize(vg, 14.0f);
		nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
		nvgFillColor(vg, nvgRGBA(240,240,240,192));
		nvgText(vg, mPosition.x+3,mPosition.y+1, mCaption.c_str(), NULL);


	if (mStyle == GRAPH_RENDER_FPS) {
		nvgFontSize(vg, 18.0f);
		nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_TOP);
		nvgFillColor(vg, nvgRGBA(240,240,240,255));
		sprintf(str, "%.2f FPS", 1.0f / avg);
		nvgText(vg, mPosition.x+mSize.x-3,mPosition.y+1, str, NULL);

		nvgFontSize(vg, 15.0f);
		nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_BOTTOM);
		nvgFillColor(vg, nvgRGBA(240,240,240,160));
		sprintf(str, "%.2f ms", avg * 1000.0f);
		nvgText(vg, mPosition.x+mSize.x-3,mPosition.y+mSize.y-1, str, NULL);
	}
	else if (mStyle == GRAPH_RENDER_PERCENT) {
		nvgFontSize(vg, 18.0f);
		nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_TOP);
		nvgFillColor(vg, nvgRGBA(240,240,240,255));
		sprintf(str, "%.1f %%", avg * 1.0f);
		nvgText(vg, mPosition.x+mSize.x-3,mPosition.y+1, str, NULL);
	} else {
		nvgFontSize(vg, 18.0f);
		nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_TOP);
		nvgFillColor(vg, nvgRGBA(240,240,240,255));
		sprintf(str, "%.2f ms", avg * 1000.0f);
		nvgText(vg, mPosition.x+mSize.x-3,mPosition.y+1, str, NULL);
	}

  double t = glfwGetTime();
  float dt = t - prevt;
  prevt = t;
  updateGraph(dt);
}


bool PerfGraph::mouseButtonEvent(const ivec2 &p, int button, bool down, int modifiers) {
    GUIObject::mouseButtonEvent(p, button, down, modifiers);
    /* Temporarily increase the reference count of the button in case the
       button causes the parent window to be destructed */

    return false;

}

