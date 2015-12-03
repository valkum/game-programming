#ifndef COMMON_HH
#define COMMON_HH
  
#include <array>
#include <glm/glm.hpp>


using namespace glm;

struct NVGcontext;
struct NVGcolor;
struct GLFWwindow;

class Color : public vec4 {
public:
  Color() : Color(0, 0, 0, 0) { }
  Color(const vec4 &color) : vec4(color) { }
  Color(const vec3 &color, float alpha) : Color(color.r, color.g, color.b, alpha) { }
  Color(float r, float g, float b, float alpha) : Color(vec4(r, g, b, alpha)) { }
  Color(int r, int g, int b, int a) : Color(vec4((float)r, (float)g, (float)b, (float)a)) { }

  inline operator const NVGcolor &() const;
  // NVGcolor data();
};

struct Theme {
  Color mDropShadow                   = Color(  0.0f,   0.0f,   0.0f, 128.0f);
  Color mTransparent                  = Color(  0.0f,   0.0f,   0.0f,   0.0f);
  Color mBorderDark                   = Color( 29.0f,  29.0f,  29.0f, 255.0f);
  Color mBorderLight                  = Color( 92.0f,  92.0f,  92.0f, 255.0f);
  Color mBorderMedium                 = Color( 35.0f,  35.0f,  35.0f, 255.0f);
  Color mTextColor                    = Color(255.0f, 255.0f, 255.0f, 160.0f);
  Color mDisabledTextcolor            = Color(255.0f, 255.0f, 255.0f,  80.0f);
  Color mTextColorShadow              = Color(  0.0f,   0.0f,   0.0f, 160.0f);
  Color mIconColor                    = Color(255.0f, 255.0f, 255.0f, 160.0f);
  Color mButtonGradientTopFocused     = Color( 64.0f,  64.0f,  64.0f, 255.0f);
  Color mButtonGradientBotFocused     = Color( 48.0f,  48.0f,  48.0f, 255.0f);
  Color mButtonGradientTopUnfocused   = Color( 74.0f,  74.0f,  74.0f, 255.0f);
  Color mButtonGradientBotUnfocused   = Color( 58.0f,  58.0f,  58.0f, 255.0f);
  Color mButtonGradientTopPushed      = Color( 41.0f,  41.0f,  41.0f, 255.0f);
  Color mButtonGradientBotPushed      = Color( 29.0f,  29.0f,  29.0f, 255.0f);

  int mStandardFontSize                 = 16;
  int mButtonFontSize                   = 20;
  int mTextBoxFontSize                  = 20;
  int mWindowCornerRadius               = 2;
  int mWindowHeaderHeight               = 30;
  int mWindowDropShadowSize             = 10;
  int mButtonCornerRadius               = 2;
};

extern struct Theme Theme;

extern std::array<char, 8> utf8(int c);

#endif // COMMON_HH