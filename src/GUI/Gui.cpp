#include "GUI/Gui.hh"
#include "GUI/nanovg.hh"
#include "Helper.hh"
#include <ACGL/Utils/Log.hh>


#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

using namespace std;
using namespace ACGL::Utils;


Gui::Gui(NVGcontext *ctx, GLFWwindow * window) : GUIObject(nullptr) {
  if(ctx == nullptr)
    ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
  mNVGContext = ctx;
  mGLFWWindow = window;

  int mFontNormal =
    nvgCreateFont(ctx, "sans",
                  (Helper::getExePath() +
                   "/assets/fonts/Roboto-Regular.ttf").c_str());

  if (mFontNormal == -1) {
    debug()  << "Could not load Roboto-Regular.ttf from " <<
    (Helper::getExePath() + "/assets/fonts/Roboto-Regular.ttf") << std::endl;
  }
  int mFontBold =
    nvgCreateFont(ctx, "sans-bold",
                  (Helper::getExePath() +
                   "/assets/fonts/Roboto-Bold.ttf").c_str());

  if (mFontBold == -1) {
    debug()  << "Could not load Roboto-Bold.ttf from " <<
    (Helper::getExePath() + "/assets/fonts/Roboto-Bold.ttf") << std::endl;
  }
  int mFontItem =
    nvgCreateFont(ctx, "icons",
                  (Helper::getExePath() + "/assets/fonts/entypo.ttf").c_str());

  if (mFontItem == -1) {
    debug()  << "Could not load entypo.ttf from " <<
    (Helper::getExePath() + "/assets/fonts/entypo.ttf") << std::endl;
  }

}

Gui::~Gui() {
  if(mNVGContext) {
    nvgDeleteGL3(mNVGContext);
  }
}

void Gui::drawAll() {
  glfwMakeContextCurrent(mGLFWWindow);
  glfwGetFramebufferSize(mGLFWWindow, &mFBSize.x, &mFBSize.y);
  glfwGetWindowSize(mGLFWWindow, &mSize.x, &mSize.y);
  mPixelRatio = (float) mFBSize.x / (float) mSize.x;

  nvgBeginFrame(mNVGContext, mSize.x, mSize.y, mPixelRatio);
  draw(mNVGContext);
  nvgEndFrame(mNVGContext);
}

bool Gui::keyboardEvent(int key, int scancode, int action, int mods) {
  if(mFocusPath.size() > 0) {
    for(auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); ++it) {
      if((*it)->focused() && (*it)->keyboardEvent(key, scancode, action, mods))
        return true;
    }
  }
  return false;
}
bool Gui::keyboardCharacterEvent(unsigned int codepoint){
  if(mFocusPath.size() > 0) {
    for(auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); it++) {
      if((*it)->focused() && (*it)->keyboardCharacterEvent(codepoint))
        return true;
    }
  }
  return false;
}
bool Gui::cursorPosCallbackEvent(double x, double y) {
  ivec2 p((int)x, (int)y);
  bool ret = false;
  try{
    p -= ivec2(1,2);
    if(!ret)
        ret = mouseMotionEvent(p, p - mMousePos, mMouseState, mModifiers);

    mMousePos = p;
    return ret;
  } catch( const std::exception &e) {
    debug() << "Cought exception in event handler: " << e.what() << std::endl;
  }

  return false;
}
bool Gui::mouseButtonCallbackEvent(int button, int action, int mods) {
  mModifiers = mods;
  try {
    if(action == GLFW_PRESS)
      mMouseState |= 1 << button;
    else
      mMouseState &= ~(1 << button);

    if(action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1) {
      updateFocus(nullptr);
    }

    return mouseButtonEvent(mMousePos, button, action == GLFW_PRESS, mModifiers);
  } catch ( const std::exception &e) {
    debug() << "Caught exception in event handler: " << e.what() << std::endl;
  }
  return false;
}
bool Gui::keyCallbackEvent(int key, int scancode, int action, int mods) {
  try{
    return keyboardEvent(key, scancode, action, mods);
  } catch (std::exception &e) {
    debug() << "Caught exception in event handler: " << e.what() << std::endl;
  }
  return false;
}

bool Gui::charCallbackEvent(unsigned int codepoint) {
  try{
    return keyboardCharacterEvent(codepoint);
  } catch (std::exception &e) {
    debug() << "Caught exception in event handler: " << e.what() << std::endl;
  }
  return false;
}

bool Gui::resizeCallbackEvent(int, int) {
  ivec2 fbSize, size;
  glfwGetFramebufferSize(mGLFWWindow, &fbSize.x, &fbSize.y);
  glfwGetWindowSize(mGLFWWindow, &size.x, &size.y);

  if(mFBSize == ivec2(0,0) || size == ivec2(0,0))
    return false;

  mFBSize = fbSize; mSize  = size;

  try {
    return resizeEvent(mSize);
  } catch(std::exception &e) {
    debug() << "Caught exception in event handler: " << e.what() << std::endl;
  }
  return false;
}

void Gui::updateFocus(GUIObject *child) {
  for( auto w: mFocusPath) {
    if(!w->focused())
      continue;
    w->focusEvent(false);
  }
  mFocusPath.clear();
  while (child) {
    mFocusPath.push_back(child);
    child = child->parent();
  }
  for(auto it = mFocusPath.rbegin(); it != mFocusPath.rend(); ++it)
    (*it)->focusEvent(true);
}