#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstring>
#include "Shader.hpp"
#include "build_config.h"

void Shader::loadFromString(const char* source) {
  glShaderSource(_id, 1, &source, NULL);
  glCompileShader(_id);

  GLint compileStatus;
  glGetShaderiv(_id, GL_COMPILE_STATUS, &compileStatus);

  if (!compileStatus) {
    std::string infoLog;
    GLint infoLogLength;
    glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength) {
      infoLog.resize(infoLogLength - 1);
      glGetShaderInfoLog(_id, infoLogLength, NULL, infoLog.data());
    }
    std::string msg = Shader::getShaderTypeStr(_type);
    msg[0] = toupper(msg[0]);
    msg = msg + " shader compilation failed: " + infoLog;
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

void ShaderProgram::link() {
  glLinkProgram(_id);
  GLint programLinkStatus;
  glGetProgramiv(_id, GL_LINK_STATUS, &programLinkStatus);
  
  if (!programLinkStatus) {
    std::string infoLog;
    GLint infoLogLength;
    glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength) {
      infoLog.resize(infoLogLength - 1);
      glGetProgramInfoLog(_id, infoLogLength, NULL, infoLog.data());
    }
    std::string msg = "Program linking failed: " + infoLog;
    throw ShaderException(msg);
  }
}
