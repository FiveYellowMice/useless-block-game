#ifndef _SHADER_HPP_
#define _SHADER_HPP_
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ApplicationException.hpp"

class Shader {
public:
  GLuint id;
  GLenum type;

  Shader(GLenum type_);
  ~Shader();

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  void loadFromString(const char* source);
  void loadFromString(const std::string& source);
  void loadFromFile(const std::string& filename);

  static const char* getShaderTypeStr(GLenum shaderType);
};

class ShaderException : public ApplicationException {
  using ApplicationException::ApplicationException;
};

#endif
