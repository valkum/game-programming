#ifndef TEXT_HH
#define TEXT_HH

#include "GUI/GUIObject.hh"

#include <functional>
#include <string>
#include <ACGL/Scene/GenericCamera.hh>

class Text : public GUIObject {
public:

    Text(GUIObject *parent, const std::string &caption = "Untitled");

    const std::string &caption() const { return mCaption; }
    void setCaption(const std::string &caption) { mCaption = caption; }

    const Color &textColor() const { return mTextColor; }
    void setTextColor(const Color &textColor) { mTextColor = textColor; }

    const float &fontSize() const { return mFontSize; }
    void setFontSize(const float &fontSize) { mFontSize = fontSize; }


    virtual void draw(NVGcontext *ctx);
protected:
    std::string mCaption;
    bool mEnabled = true;
    Color mTextColor;
    vec3 mCameraDirection, mCameraPosition;
    float mFontSize = -1;
};

#endif // TEXT_HH
