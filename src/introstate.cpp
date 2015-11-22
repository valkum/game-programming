#include "introstate.h"
#include <ACGL/OpenGL/Creator/ShaderProgramCreator.hh>
#include <ACGL/OpenGL/Objects.hh>
#include <ACGL/Base/Settings.hh>

#include "playstate.h"
#include <iostream>
#include "helper.h"

#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

using namespace glm;
using namespace std;
using namespace ACGL::OpenGL;
using namespace ACGL::Base;
using namespace ACGL::Utils;

IntroState IntroState::m_IntroState;
NVGcontext* vg;


// Returns 1 if col.rgba is 0.0f,0.0f,0.0f,0.0f, 0 otherwise
int isBlack(NVGcolor col)
{
    if( col.r == 0.0f && col.g == 0.0f && col.b == 0.0f && col.a == 0.0f )
    {
        return 1;
    }
    return 0;
}

static char* cpToUTF8(int cp, char* str)
{
    int n = 0;
    if (cp < 0x80) n = 1;
    else if (cp < 0x800) n = 2;
    else if (cp < 0x10000) n = 3;
    else if (cp < 0x200000) n = 4;
    else if (cp < 0x4000000) n = 5;
    else if (cp <= 0x7fffffff) n = 6;
    str[n] = '\0';
    switch (n) {
    case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
    case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
    case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
    case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
    case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
    case 1: str[0] = cp;
    }
    return str;
}

void drawButton(NVGcontext* vg, int preicon, const char* text, float x, float y, float w, float h, NVGcolor col)
{
    NVGpaint bg;
    char icon[8];
    float cornerRadius = 4.0f;
    float tw = 0, iw = 0;

    bg = nvgLinearGradient(vg, x,y,x,y+h, nvgRGBA(255,255,255,isBlack(col)?16:32), nvgRGBA(0,0,0,isBlack(col)?16:32));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+1,y+1, w-2,h-2, cornerRadius-1);
    if (!isBlack(col)) {
        nvgFillColor(vg, col);
        nvgFill(vg);
    }
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
    nvgStroke(vg);

    nvgFontSize(vg, 20.0f);
    nvgFontFace(vg, "sans-bold");
    tw = nvgTextBounds(vg, 0,0, text, NULL, NULL);
    if (preicon != 0) {
        nvgFontSize(vg, h*1.3f);
        nvgFontFace(vg, "icons");
        iw = nvgTextBounds(vg, 0,0, cpToUTF8(preicon,icon), NULL, NULL);
        iw += h*0.15f;
    }

    if (preicon != 0) {
        nvgFontSize(vg, h*1.3f);
        nvgFontFace(vg, "icons");
        nvgFillColor(vg, nvgRGBA(255,255,255,96));
        nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
        nvgText(vg, x+w*0.5f-tw*0.5f-iw*0.75f, y+h*0.5f, cpToUTF8(preicon,icon), NULL);
    }

    nvgFontSize(vg, 20.0f);
    nvgFontFace(vg, "sans-bold");
    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgFillColor(vg, nvgRGBA(0,0,0,160));
    nvgText(vg, x+w*0.5f-tw*0.5f+iw*0.25f,y+h*0.5f-1,text, NULL);
    nvgFillColor(vg, nvgRGBA(255,255,255,160));
    nvgText(vg, x+w*0.5f-tw*0.5f+iw*0.25f,y+h*0.5f,text, NULL);
}
class IntroState;
void IntroState::Init(CGame* game) {

    vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
    //@todo auslagern in gui classe.
    int fontNormal = nvgCreateFont(vg, "sans", (helper::getExePath() + "/assets/fonts/Roboto-Regular.ttf").c_str());
    if(fontNormal == -1) {
        std::cout<<"Could not load Roboto-Regular.ttf from "<<(helper::getExePath() + "/assets/fonts/Roboto-Regular.ttf")<<std::endl;
    }
    int fontBold = nvgCreateFont(vg, "sans-bold", (helper::getExePath() + "/assets/fonts/Roboto-Bold.ttf").c_str());
    if(fontBold == -1) {
        std::cout<<"Could not load Roboto-Bold.ttf from "<<(helper::getExePath() + "/assets/fonts/Roboto-Bold.ttf")<<std::endl;
    }
    int fontItem = nvgCreateFont(vg, "icons", (helper::getExePath() + "/assets/fonts/entypo.ttf").c_str());
    if(fontItem == -1) {
        std::cout<<"Could not load entypo.ttf from "<<(helper::getExePath() + "/assets/fonts/entypo.ttf")<<std::endl;
    }

}

void IntroState::Draw(CGame* game, float* delta) {



    double mx, my;
    int winWidth, winHeight;
    int fbWidth, fbHeight;
    float pxRatio;
    glfwGetCursorPos(game->g_window, &mx, &my);
    glfwGetWindowSize(game->g_window, &winWidth, &winHeight);
    glfwGetFramebufferSize(game->g_window, &fbWidth, &fbHeight);
    // Calculate pixel ration for hi-dpi devices.
    pxRatio = (float)fbWidth / (float)winWidth;
    glViewport(0,0, fbWidth, fbHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
    drawButton(vg, 0, "Play", 200, 170, 140, 28, nvgRGBA(0,96,128,255));
    drawButton(vg, 0, "Exit", 200, 210, 140, 28, nvgRGBA(0,0,0,0));

    nvgEndFrame(vg);

}

// void HandleKeyEvents(CGame* game, GLFWwindow* window, int key, int scancode, int action, int mods) {
//     CGameState::HandleKeyEvents(game, window, key, scancode, action, mods);
//     ...
// }
void IntroState::HandleMouseButtonEvents(GLFWwindow* window, vec2 mousePos, int button, int action, int mods) {
    if(action == GLFW_PRESS) {
        m_MouseState |= 1 << button;
    } else {
        m_MouseState &= ~(1 << button);
    }
    vec2 size = vec2(140, 28);
    vec2 d = (mousePos - vec2(200, 170));

    if(all(greaterThanEqual(d, vec2(0))) && all(lessThan(d, size))){
        cout<<"Button Play Clicked"<<endl;
        CGame* g = CGame::Instance();
        g->ChangeState(PlayState::Instance());
    }
    d = (mousePos - vec2(200, 210));
    if(all(greaterThanEqual(d, vec2(0))) && all(lessThan(d, size))){
        cout<<"Button Exit Clicked"<<endl;
        glfwSetWindowShouldClose( window, GL_TRUE );
    }
    

};
void IntroState::HandleMouseMoveEvents(GLFWwindow* window, vec2 mousePos){

};