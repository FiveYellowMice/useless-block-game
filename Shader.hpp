#ifndef _SHADER_HPP_
#define _SHADER_HPP_
#include <string>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ApplicationException.hpp"

class Shader {
private:
  GLuint _id;
  GLenum _type;

public:
  Shader(GLenum type_) : _type(type_) {
    _id = glCreateShader(_type);
  }
  ~Shader() {
    glDeleteShader(_id);
  }

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  GLuint id() const { return _id; };
  GLenum type() const { return _type; }

  void loadFromString(const char* source);
  void loadFromString(const std::string& source);
  void loadFromFile(const std::string& filename);

  static const char* getShaderTypeStr(GLenum shaderType);
};

template <typename C, typename E>
concept ShaderUniformArray = std::is_same_v<E, typename C::value_type>;

class ShaderProgram {
private:
  GLuint _id;
  std::vector<std::shared_ptr<Shader>> _shaders;

public:
  ShaderProgram() {
    _id = glCreateProgram();
  }
  ~ShaderProgram() {
    glDeleteProgram(_id);
  }

  ShaderProgram(const ShaderProgram&) = delete;
  ShaderProgram& operator=(const ShaderProgram&) = delete;

  GLuint id() const { return _id; }

  void attachShader(std::shared_ptr<Shader> shader) {
    _shaders.push_back(shader);
    glAttachShader(_id, shader->id());
  }

  void loadAndAttachShader(GLenum shaderType, const std::string& filename) {
    auto shader = std::make_shared<Shader>(shaderType);
    shader->loadFromFile(filename);
    attachShader(shader);
  }

  void link();

  void use() {
    glUseProgram(_id);
  }

  GLint getUniformLocation(const std::string& name) {
    return glGetUniformLocation(_id, name.c_str());
  }

  GLint getAttribLocation(const std::string& name) {
    return glGetAttribLocation(_id, name.c_str());
  }

  void setUniform(const std::string& name, float v0) { glUniform1f(getUniformLocation(name), v0); }
  void setUniform(const std::string& name, float v0, float v1) { glUniform2f(getUniformLocation(name), v0, v1); }
  void setUniform(const std::string& name, float v0, float v1, float v2) { glUniform3f(getUniformLocation(name), v0, v1, v2); }
  void setUniform(const std::string& name, float v0, float v1, float v2, float v3) { glUniform4f(getUniformLocation(name), v0, v1, v2, v3); }

  void setUniform(const std::string& name, GLint v0) { glUniform1i(getUniformLocation(name), v0); }
  void setUniform(const std::string& name, GLint v0, GLint v1) { glUniform2i(getUniformLocation(name), v0, v1); }
  void setUniform(const std::string& name, GLint v0, GLint v1, GLint v2) { glUniform3i(getUniformLocation(name), v0, v1, v2); }
  void setUniform(const std::string& name, GLint v0, GLint v1, GLint v2, GLint v3) { glUniform4i(getUniformLocation(name), v0, v1, v2, v3); }

  void setUniform(const std::string& name, GLuint v0) { glUniform1ui(getUniformLocation(name), v0); }
  void setUniform(const std::string& name, GLuint v0, GLuint v1) { glUniform2ui(getUniformLocation(name), v0, v1); }
  void setUniform(const std::string& name, GLuint v0, GLuint v1, GLuint v2) { glUniform3ui(getUniformLocation(name), v0, v1, v2); }
  void setUniform(const std::string& name, GLuint v0, GLuint v1, GLuint v2, GLuint v3) { glUniform4ui(getUniformLocation(name), v0, v1, v2, v3); }

  template <ShaderUniformArray<float> C> void setUniform(const std::string& name, const C& v) { glUniform1fv(getUniformLocation(name), v.size(), v.data()); }
  template <ShaderUniformArray<glm::vec2> C> void setUniform(const std::string& name, const C& v) { glUniform2fv(getUniformLocation(name), v.size(), glm::value_ptr(*v.data())); }
  template <ShaderUniformArray<glm::vec3> C> void setUniform(const std::string& name, const C& v) { glUniform3fv(getUniformLocation(name), v.size(), glm::value_ptr(*v.data())); }
  template <ShaderUniformArray<glm::vec4> C> void setUniform(const std::string& name, const C& v) { glUniform4fv(getUniformLocation(name), v.size(), glm::value_ptr(*v.data())); }

  void setUniform(const std::string& name, const glm::vec2& v) { glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(v)); }
  void setUniform(const std::string& name, const glm::vec3& v) { glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(v)); }
  void setUniform(const std::string& name, const glm::vec4& v) { glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(v)); }

  template <ShaderUniformArray<GLint> C> void setUniform(const std::string& name, const C& v) { glUniform1iv(getUniformLocation(name), v.size(), v.data()); }
  template <ShaderUniformArray<glm::ivec2> C> void setUniform(const std::string& name, const C& v) { glUniform2iv(getUniformLocation(name), v.size(), glm::value_ptr(*v.data())); }
  template <ShaderUniformArray<glm::ivec3> C> void setUniform(const std::string& name, const C& v) { glUniform3iv(getUniformLocation(name), v.size(), glm::value_ptr(*v.data())); }
  template <ShaderUniformArray<glm::ivec4> C> void setUniform(const std::string& name, const C& v) { glUniform4iv(getUniformLocation(name), v.size(), glm::value_ptr(*v.data())); }

  void setUniform(const std::string& name, const glm::ivec2& v) { glUniform2iv(getUniformLocation(name), 1, glm::value_ptr(v)); }
  void setUniform(const std::string& name, const glm::ivec3& v) { glUniform3iv(getUniformLocation(name), 1, glm::value_ptr(v)); }
  void setUniform(const std::string& name, const glm::ivec4& v) { glUniform4iv(getUniformLocation(name), 1, glm::value_ptr(v)); }

  template <ShaderUniformArray<GLuint> C> void setUniform(const std::string& name, const C& v) { glUniform1uiv(getUniformLocation(name), v.size(), v.data()); }
  template <ShaderUniformArray<glm::uvec2> C> void setUniform(const std::string& name, const C& v) { glUniform2uiv(getUniformLocation(name), v.size(), glm::value_ptr(*v.data())); }
  template <ShaderUniformArray<glm::uvec3> C> void setUniform(const std::string& name, const C& v) { glUniform3uiv(getUniformLocation(name), v.size(), glm::value_ptr(*v.data())); }
  template <ShaderUniformArray<glm::uvec4> C> void setUniform(const std::string& name, const C& v) { glUniform4uiv(getUniformLocation(name), v.size(), glm::value_ptr(*v.data())); }

  void setUniform(const std::string& name, const glm::uvec2& v) { glUniform2uiv(getUniformLocation(name), 1, glm::value_ptr(v)); }
  void setUniform(const std::string& name, const glm::uvec3& v) { glUniform3uiv(getUniformLocation(name), 1, glm::value_ptr(v)); }
  void setUniform(const std::string& name, const glm::uvec4& v) { glUniform4uiv(getUniformLocation(name), 1, glm::value_ptr(v)); }

  void setUniform(const std::string& name, const glm::mat2& v) { glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v)); }
  void setUniform(const std::string& name, const glm::mat3& v) { glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v)); }
  void setUniform(const std::string& name, const glm::mat4& v) { glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v)); }
};

class ShaderException : public ApplicationException {
  using ApplicationException::ApplicationException;
};

#endif
