#include "GUI/GUIObject.hh"

#include "GUI/nanovg.hh"
#include <algorithm>

using namespace glm;

class GUIObject;
GUIObject::GUIObject(GUIObject* parent)
    : mParent(nullptr),  mPosition(vec2(0)), mSize(vec2(0)),
      mFontSize(-1), mFocused(false), mMouseFocus(false) {
        if(parent) {
            parent->addChild(this);
        } 
}

GUIObject::~GUIObject() {
    for (auto child : mChildren) {
        if(child)
            delete child;
    }
}


GUIObject *GUIObject::findChild(const ivec2 &p) {
    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
        GUIObject *child = *it;
        if (child->contains(p - mPosition))
            return child->findChild(p - mPosition);
    }
    return contains(p) ? this : nullptr;
}

bool GUIObject::mouseButtonEvent(const ivec2 &p, int button, bool down, int modifiers) {
    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
        GUIObject *child = *it;
        if (child->contains(p - mPosition) &&
            child->mouseButtonEvent(p - mPosition, button, down, modifiers))
            return true;
    }
    return false;
}

bool GUIObject::mouseMotionEvent(const ivec2 &p, const ivec2 &rel, int button, int modifiers) {
    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
        GUIObject *child = *it;
        // if (!child->visible())
        //     continue;
        bool contained = child->contains(p - mPosition), prevContained = child->contains(p - mPosition - rel);
        if (contained != prevContained)
            child->mouseEnterEvent(p, contained);
        if ((contained || prevContained) &&
            child->mouseMotionEvent(p - mPosition, rel, button, modifiers))
            return true;
    }
    return false;
}

bool GUIObject::mouseEnterEvent(const ivec2 &, bool enter) {
    mMouseFocus = enter;
    return false;
}

bool GUIObject::focusEvent(bool focused) {
    mFocused = focused;
    return false;
}

bool GUIObject::keyboardEvent(int, int, int, int) {
    return false;
}

bool GUIObject::keyboardCharacterEvent(unsigned int) {
    return false;
}

void GUIObject::addChild(GUIObject *child) {
    mChildren.push_back(child);
    child->setParent(this);
}

void GUIObject::removeChild(const GUIObject *child) {
    mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), child), mChildren.end());
    delete child;
}

// void GUIObject::removeChild(int index) {
//     GUIObject *child = mChildren[index];
//     mChildren.erase(mChildren.begin() + index);
//     delete child;
// }


void GUIObject::draw(NVGcontext *ctx) {

    if (mChildren.empty() || mRender == false)
        return;
    nvgTranslate(ctx, mPosition.x, mPosition.y);
    for (auto child : mChildren)
        child->draw(ctx);
    nvgTranslate(ctx, -mPosition.x, -mPosition.y);
}

