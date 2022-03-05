#ifndef _SHADER_HPP_
#define _SHADER_HPP_
#include <string>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ApplicationException.hpp"

class Shader {
private:
  GLuint _id;
  GLenum _type;

public:
  Shader(GLenum type_);
  ~Shader();

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  GLuint id() const { return _id; };
  GLenum type() const { return _type; }

  void loadFromString(const char* source);
  void loadFromString(const std::string& source);
  void loadFromFile(const std::string& filename);

  static const char* getShaderTypeStr(GLenum shaderType);
};

class ShaderProgram {
private:
  GLuint _id;
  std::vector<std::shared_ptr<Shader>> _shaders;

public:
  ShaderProgram();
  ~ShaderProgram();

  ShaderProgram(const ShaderProgram&) = delete;
  ShaderProgram& operator=(const ShaderProgram&) = delete;

  GLuint id() const { return _id; }

  void attachShader(std::shared_ptr<Shader> shader);
  void loadAndAttachShader(GLenum shaderType, const std::string& filename);
  void link();
  void use();
};

class ShaderException : public ApplicationException {
  using ApplicationException::ApplicationException;
};

#endif
