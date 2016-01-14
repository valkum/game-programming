#ifndef BUTTON_HH
#define BUTTON_HH

#include "GUI/GUIObject.hh"

#include <functional>
#include <string>

class Button : public GUIObject {
public:
    enum class IconPosition {
        Left,
        LeftCentered,
        RightCentered,
        Right
    };

    Button(GUIObject *parent, const std::string &caption = "Untitled", int icon = 0);

    const std::string &caption() const { return mCaption; }
    void setCaption(const std::string &caption) { mCaption = caption; }

    const Color &backgroundColor() const { return mBackgroundColor; }
    void setBackgroundColor(const Color &backgroundColor) { mBackgroundColor = backgroundColor; }

    const Color &textColor() const { return mTextColor; }
    void setTextColor(const Color &textColor) { mTextColor = textColor; }

    int icon() const { return mIcon; }
    void setIcon(int icon) { mIcon = icon; }

    IconPosition iconPosition() const { return mIconPosition; }
    void setIconPosition(IconPosition iconPosition) { mIconPosition = iconPosition; }

    bool pushed() const { return mPushed; }
    void setPushed(bool pushed) { mPushed = pushed; }

    /// Set the push callback (for any type of button)
    std::function<void()> callback() const { return mCallback; }
    void setCallback(const std::function<void()> &callback) { mCallback = callback; }

    /// Set the change callback (for toggle buttons)
    std::function<void(bool)> changeCallback() const { return mChangeCallback; }
    void setChangeCallback(const std::function<void(bool)> &callback) { mChangeCallback = callback; }

    virtual bool mouseButtonEvent(const ivec2 &p, int button, bool down, int modifiers);
    virtual void draw(NVGcontext *ctx);
protected:
    std::string mCaption;
    int mIcon;
    IconPosition mIconPosition;
    bool mPushed, mEnabled = true;
    Color mBackgroundColor;
    Color mTextColor;
    std::function<void()> mCallback;
    std::function<void(bool)> mChangeCallback;
};

#endif // BUTTON_HH
