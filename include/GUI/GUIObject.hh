#ifndef GUI_OBJECT_HH
#define GUI_OBJECT_HH

#include "GUI/common.hh"
#include <vector>

using namespace glm;


class GUIObject {
public:
  GUIObject(GUIObject *parent);
  
  GUIObject *parent(){ return mParent; }
  const GUIObject *parent() const { return mParent; }
  void setParent(GUIObject *parent) {mParent = parent; }

  const ivec2 &position() const { return mPosition; }
  void setPosition(const ivec2 &pos) { mPosition = pos; }

  ivec2 absolutePosition() const {
    return mParent ? (parent()->absolutePosition() + mPosition) : mPosition;
  }

  const ivec2 &size() const { return mSize; }
  void setSize(const ivec2 &size) {mSize = size; }

  bool focused() const { return mFocused; }
  void setFocused(bool focused) { mFocused = focused; }
  void hide() {mRender = false; }
  void show() {mRender = true; }
  
  const std::vector<GUIObject *> &children() const {return mChildren; }
  void addChild(GUIObject *child);
  void removeChild(const GUIObject *child);
  GUIObject *findChild(const ivec2 &p);

  bool contains(const ivec2 &p) const {
    auto d = (p - mPosition);
    return all(greaterThanEqual(d, ivec2(0))) && all(lessThan(d, mSize));
  }

  /// Handle a mouse button event (default implementation: propagate to children)
  virtual bool mouseButtonEvent(const ivec2 &p, int button, bool down, int modifiers);

  /// Handle a mouse motion event (default implementation: propagate to children)
  virtual bool mouseMotionEvent(const ivec2 &p, const ivec2 &rel, int button, int modifiers);

  /// Handle a mouse drag event (default implementation: do nothing)
  //virtual bool mouseDragEvent(const ivec2 &p, const ivec2 &rel, int button, int modifiers);

  /// Handle a mouse enter/leave event (default implementation: record this fact, but do nothing)
  virtual bool mouseEnterEvent(const ivec2 &p, bool enter);

  /// Handle a mouse scroll event (default implementation: propagate to children)
  //virtual bool scrollEvent(const ivec2 &p, const vec2 &rel);

  /// Handle a focus change event (default implementation: record the focus status, but do nothing)
  virtual bool focusEvent(bool focused);

  /// Handle a keyboard event (default implementation: do nothing)
  virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);

  /// Handle text input (UTF-32 format) (default implementation: do nothing)
  virtual bool keyboardCharacterEvent(unsigned int codepoint);

  virtual void draw(NVGcontext *ctx);
protected:
  virtual ~GUIObject();

protected:
  GUIObject *mParent;
  ivec2 mPosition, mSize;
  int mFontSize;
  std::vector<GUIObject *> mChildren;
  bool mFocused, mMouseFocus;
  bool mRender = true;
};


#endif // GUI_OBJECT_HH
