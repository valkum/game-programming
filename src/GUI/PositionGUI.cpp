#include "GUI/PositionGUI.hh"

#include "GUI/nanovg.hh"
#include <ACGL/Utils/Log.hh>
#include <ACGL/Scene/GenericCamera.hh>
#include <glm/ext.hpp>

PositionGUI::PositionGUI(GUIObject *parent, const std::string &caption)
    : GUIObject(parent), mCaption(caption), mTextColor(Color(0,255)) { }

void PositionGUI::draw(NVGcontext *ctx) {
    GUIObject::draw(ctx);
    if(!mRender) {
        return;
    }
    std::string str;
    int fontSize = mFontSize == -1 ? Theme.mStandardFontSize : mFontSize;

    nvgBeginPath(ctx);

    nvgFontSize(ctx, fontSize);
    nvgFontFace(ctx, "sans-bold");
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

    str = "Pos: " + to_string(mCameraPosition);
    nvgFillColor(ctx, Theme.mTextColor);
    nvgText(ctx, mPosition.x, mPosition.y, str.c_str(), nullptr);

    str = "Angle: " + to_string(mCameraDirection);

    nvgFillColor(ctx, Theme.mTextColor);
    nvgText(ctx, mPosition.x, mPosition.y + fontSize + 2, str.c_str(), nullptr);

}
