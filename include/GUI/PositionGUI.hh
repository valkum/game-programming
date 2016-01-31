#ifndef POSITIONGUI_HH
#define POSITIONGUI_HH

#include "GUI/GUIObject.hh"

#include <functional>
#include <string>
#include <ACGL/Scene/GenericCamera.hh>

class PositionGUI : public GUIObject {
public:

    PositionGUI(GUIObject *parent, const std::string &caption = "Untitled");

    const std::string &caption() const { return mCaption; }
    void setCaption(const std::string &caption) { mCaption = caption; }

    const Color &textColor() const { return mTextColor; }
    void setTextColor(const Color &textColor) { mTextColor = textColor; }

    void setCameraDirection(vec3 direction) {mCameraDirection = direction; }
    void setCameraPosition(vec3 position) {mCameraPosition = position; }

    virtual void draw(NVGcontext *ctx);
protected:
    std::string mCaption;
    bool mEnabled = true;
    Color mTextColor;
    vec3 mCameraDirection, mCameraPosition;
};

#endif // POSITIONGUI_HH
