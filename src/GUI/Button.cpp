#include "GUI/Button.hh"

#include "GUI/nanovg.hh"
#include <ACGL/Utils/Log.hh>

Button::Button(GUIObject *parent, const std::string &caption, int icon)
    : GUIObject(parent), mCaption(caption), mIcon(icon),
      mIconPosition(IconPosition::LeftCentered), mPushed(false),
      mBackgroundColor(Color(0,0,0,0)), mTextColor(Color(0,0,0,0)) { }

bool Button::mouseButtonEvent(const ivec2 &p, int button, bool down, int modifiers) {
    GUIObject::mouseButtonEvent(p, button, down, modifiers);
    /* Temporarily increase the reference count of the button in case the
       button causes the parent window to be destructed */

    if (button == GLFW_MOUSE_BUTTON_1 && mEnabled) {
        bool pushedBackup = mPushed;
        if (down) {
            mPushed = true;
        } else if (mPushed) {
            if (contains(p) && mCallback)
                mCallback();
                mPushed = false;
        }
        if (pushedBackup != mPushed && mChangeCallback)
            mChangeCallback(mPushed);

        return true;
    }
    return false;
}

void Button::draw(NVGcontext *ctx) {
    GUIObject::draw(ctx);


    NVGcolor gradTop = Theme.mButtonGradientTopUnfocused;
    NVGcolor gradBot = Theme.mButtonGradientBotUnfocused;

    if (mPushed) {
        gradTop = Theme.mButtonGradientTopPushed;
        gradBot = Theme.mButtonGradientBotPushed;
    } else if (mMouseFocus && mEnabled) {
        gradTop = Theme.mButtonGradientTopFocused;
        gradBot = Theme.mButtonGradientBotFocused;
    }

    nvgBeginPath(ctx);

    nvgRoundedRect(ctx, mPosition.x + 1, mPosition.y + 1.0f, mSize.x - 2,
                   mSize.y - 2, Theme.mButtonCornerRadius - 1);

    if (mBackgroundColor.a != 0) {
        nvgFillColor(ctx, nvgRGBA(mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, 1.f));
        nvgFill(ctx);
        if (mPushed) {
            gradTop.a = gradBot.a = 0.8f;
        } else {
            double v = 1 - mBackgroundColor.a;
            gradTop.a = gradBot.a = mEnabled ? v : v * .5f + .5f;
        }
    }

    NVGpaint bg = nvgLinearGradient(ctx, mPosition.x, mPosition.y, mPosition.x,
                                    mPosition.y + mSize.y, gradTop, gradBot);
    nvgFillPaint(ctx, bg);
    nvgFill(ctx);

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, mPosition.x + 0.5f, mPosition.y + (mPushed ? 0.5f : 1.5f), mSize.x - 1,
                   mSize.y - 1 - (mPushed ? 0.0f : 1.0f), Theme.mButtonCornerRadius);
    nvgStrokeColor(ctx, Theme.mBorderLight);
    nvgStroke(ctx);

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, mPosition.x + 0.5f, mPosition.y + 0.5f, mSize.x - 1,
                   mSize.y - 2, Theme.mButtonCornerRadius);
    nvgStrokeColor(ctx, Theme.mBorderDark);
    nvgStroke(ctx);

    int fontSize = mFontSize == -1 ? Theme.mButtonFontSize : mFontSize;
    nvgFontSize(ctx, fontSize);
    nvgFontFace(ctx, "sans-bold");
    float tw = nvgTextBounds(ctx, 0,0, mCaption.c_str(), nullptr, nullptr);

    vec2 center = (vec2)mPosition + ((vec2)mSize) * 0.5f;
    vec2 textPos(center.x - 0.5f * tw, center.y - 1);
    NVGcolor textColor =
        mTextColor.a == 0 ? Theme.mTextColor : mTextColor;
    // if (!mEnabled)
    //     textColor = mDisabledTextColor;

    if (mIcon) {
        auto icon = utf8(mIcon);

        float iw, ih = fontSize;
        ih *= 1.5f;
        nvgFontSize(ctx, ih);
        nvgFontFace(ctx, "icons");
        iw = nvgTextBounds(ctx, 0, 0, icon.data(), nullptr, nullptr);
        
        if (mCaption != "")
            iw += mSize.y * 0.15f;
        nvgFillColor(ctx, textColor);
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        vec2 iconPos = center;
        iconPos.y -= 1;

        if (mIconPosition == IconPosition::LeftCentered) {
            iconPos.x -= (tw + iw) * 0.5f;
            textPos.x += iw * 0.5f;
        } else if (mIconPosition == IconPosition::RightCentered) {
            textPos.x -= iw * 0.5f;
            iconPos.x += tw * 0.5f;
        } else if (mIconPosition == IconPosition::Left) {
            iconPos.x = mPosition.x + 8;
        } else if (mIconPosition == IconPosition::Right) {
            iconPos.x = mPosition.x + mSize.x - iw - 8;
        }

        nvgText(ctx, iconPos.x, iconPos.y+1, icon.data(), nullptr);
    }

    nvgFontSize(ctx, fontSize);
    nvgFontFace(ctx, "sans-bold");
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(ctx, Theme.mTextColorShadow);
    nvgText(ctx, textPos.x, textPos.y, mCaption.c_str(), nullptr);
    nvgFillColor(ctx, textColor);
    nvgText(ctx, textPos.x, textPos.y + 1, mCaption.c_str(), nullptr);
}
