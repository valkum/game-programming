#ifndef GUIBASE_H
#define GUIBASE_H
#include "GUI/common.hh"
#include "GUI/GUIObject.hh"

#include <string>
#include <array>
#include <vector>

using namespace glm;
using namespace std;

class Gui : public GUIObject {
  friend class GUIObject;
public:
  Gui(NVGcontext *ctx, GLFWwindow *window);

  /// Free all resources used by the widget and any children
  ~Gui();

  void drawAll();
  /// Default keyboard event handler
  bool keyboardEvent(int key, int scancode, int action, int modifiers);

  /// Text input event handler: codepoint is native endian UTF-32 format
  bool keyboardCharacterEvent(unsigned int codepoint);

  /// Window resize event handler
  bool resizeEvent(const ivec2 &) { return false; }

  bool cursorPosCallbackEvent(double x, double y);
  bool mouseButtonCallbackEvent(int button, int action, int mods);
  bool keyCallbackEvent(int key, int scancode, int action, int mods);
  bool charCallbackEvent(unsigned int codepoint);
  bool resizeCallbackEvent(int width, int height);

  void updateFocus(GUIObject *child);

  NVGcontext* getContext(){
    return mNVGContext;
  };

  ivec2 mousePosition() {return mMousePos; }

protected:
  
  GLFWwindow *mGLFWWindow;
  NVGcontext *mNVGContext;
  std::vector<GUIObject *> mFocusPath;
  ivec2 mFBSize;
  float mPixelRatio;
  int mMouseState, mModifiers;
  ivec2 mMousePos;

};



#endif
