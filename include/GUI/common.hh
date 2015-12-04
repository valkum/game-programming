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
  Color(const ivec3 &color, int alpha) : Color((vec3)color / 255.f, alpha / 255.f) { }
  Color(const ivec4 &color) : Color((vec4)color / 255.f) { }
  Color(float r, float g, float b, float alpha) : Color(vec4(r, g, b, alpha)) { }
  Color(int r, int g, int b, int a) : Color(vec4(((float)r)/255.0f, ((float)g)/255.0f, ((float)b)/255.0f, ((float)a)/255.0f)) { }
  Color(float intensity, float alpha) : Color(vec3(intensity), alpha) { }
  Color(int intensity, int alpha) : Color(ivec3(intensity), alpha) { }

  inline operator const NVGcolor &() const;
  // NVGcolor data();
};

struct Theme {
  Color mDropShadow                   = Color(  0,   0,   0, 128);
  Color mTransparent                  = Color(  0,   0,   0,   0);
  Color mBorderDark                   = Color( 29,  29,  29, 255);
  Color mBorderLight                  = Color( 92,  92,  92, 255);
  Color mBorderMedium                 = Color( 35,  35,  35, 255);
  Color mTextColor                    = Color(255, 255, 255, 160);
  Color mDisabledTextcolor            = Color(255, 255, 255,  80);
  Color mTextColorShadow              = Color(  0,   0,   0, 160);
  Color mIconColor                    = Color(255, 255, 255, 160);
  Color mButtonGradientTopFocused     = Color( 64,  64,  64, 255);
  Color mButtonGradientBotFocused     = Color( 48,  48,  48, 255);
  Color mButtonGradientTopUnfocused   = Color( 74,  74,  74, 255);
  Color mButtonGradientBotUnfocused   = Color( 58,  58,  58, 255);
  Color mButtonGradientTopPushed      = Color( 41,  41,  41, 255);
  Color mButtonGradientBotPushed      = Color( 29,  29,  29, 255);

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