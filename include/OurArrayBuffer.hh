#ifndef OUR_ARRAY_BUFFER_HH
#define OUR_ARRAY_BUFFER_HH
#include <ACGL/ACGL.hh>

#include <ACGL/Base/Macros.hh>
#include <ACGL/OpenGL/GL.hh>
#include <ACGL/OpenGL/Tools.hh>

#include <ACGL/OpenGL/Objects/ArrayBuffer.hh>

namespace ACGL{
namespace OpenGL{


class OurArrayBuffer : public ArrayBuffer {
public:
  void setSize(GLint64  size) {mSize = size;};
};
ACGL_SMARTPOINTER_TYPEDEFS(OurArrayBuffer)
}}


#endif // OUR_ARRAY_BUFFER_HH
