#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ApplicationException.hpp"

class Texture {
public:
  GLuint id;
  GLenum type;

  Texture(GLenum type_);

  void bind();
  void loadFromFile(const std::string& filename);
};

class TextureException : public ApplicationException {
  using ApplicationException::ApplicationException;
};

#endif
