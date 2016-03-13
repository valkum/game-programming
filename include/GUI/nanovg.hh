#ifndef NANOVG_HH
#define NANOVG_HH

#include "GUI/common.hh"

#include <ACGL/OpenGL/GL.hh>
#include <GLFW/glfw3.h>
#include <nanovg.h>



inline Color::operator const NVGcolor &() const {
  return reinterpret_cast<const NVGcolor &>(*this);
}

// NVGcolor Color::data() {
//   NVGcolor color;
//   color.r = this->x;
//   color.g = this->y;
//   color.b = this->z;
//   color.a = this->w;
//   return color;
// }

#endif // NANOVG_HH