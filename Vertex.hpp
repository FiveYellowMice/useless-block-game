#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_
#include <GL/glew.h>

struct Vertex {
  float x, y, z;
  float nx, ny, nz;
  float u, v;
  GLuint tx, ty;
};

#endif
