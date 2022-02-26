#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstring>
#include "Shader.hpp"
#include "build_config.h"

Shader::Shader(GLenum type_) {
  type = type_;
  id = glCreateShader(type);
}

void Shader::loadFromString(const char* source) {
  glShaderSource(id, 1, &source, NULL);
  glCompileShader(id);

  GLint compileStatus;
  glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

  if (!compileStatus) {
    GLint infoLogLength;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar* infoLog = new GLchar[infoLogLength];
    glGetShaderInfoLog(id, infoLogLength, NULL, infoLog);

    std::string msg = Shader::getShaderTypeStr(type);
    msg[0] = toupper(msg[0]);
    msg = msg + " shader compilation failed: " + infoLog;
    delete[] infoLog;
    throw ShaderException(msg);
  }
}

void Shader::loadFromString(const std::string& source) {
  loadFromString(source.c_str());
}

void Shader::loadFromFile(const std::string& filename) {
  std::filesystem::path path(APP_RESOURCE_PATH);
  path.append(filename);

  std::ifstream file(path);
  if (!file.is_open()) {
    std::string msg = strerror(errno);
    throw ShaderException("Cannot open shader source file " + path.string() + ": " + msg);
  }
  std::ostringstream text;
  text << file.rdbuf();
  if (!file.good() && !file.eof()) {
    throw ShaderException("Cannot read shader source file " + path.string());
  }

  loadFromString(text.str());
}

const char* Shader::getShaderTypeStr(GLenum shaderType) {
  switch (shaderType) {
    case GL_VERTEX_SHADER:
      return "vertex";
    case GL_FRAGMENT_SHADER:
      return "fragment";
    default:
      return "";
  }
}
