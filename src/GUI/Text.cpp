#include "GUI/Text.hh"

#include "GUI/nanovg.hh"
#include <ACGL/Utils/Log.hh>
#include <glm/ext.hpp>

Text::Text(GUIObject *parent, const std::string &caption)
    : GUIObject(parent), mCaption(caption), mTextColor(Color(0,255)) { }

void Text::draw(NVGcontext *ctx) {
    GUIObject::draw(ctx);
    if(!mRender) {
        return;
    }
    int fontSize = mFontSize == -1 ? Theme.mStandardFontSize : mFontSize;

    nvgBeginPath(ctx);

    nvgFontSize(ctx, fontSize);
    nvgFontFace(ctx, "sans-bold");
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

    nvgFillColor(ctx, mTextColor);
    nvgText(ctx, mPosition.x, mPosition.y, mCaption.c_str(), nullptr);
}
