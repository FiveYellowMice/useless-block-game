#ifndef _VAO_HPP_
#define _VAO_HPP_
#include <GL/glew.h>

class VAO {
private:
  GLuint _id;

public:
  VAO() {
    glGenVertexArrays(1, &_id);
  }
  ~VAO() {
    glDeleteVertexArrays(1, &_id);
  }

  VAO(const VAO&) = delete;
  VAO& operator=(const VAO&) = delete;

  GLuint id() const { return _id; }

  void bind() {
    glBindVertexArray(_id);
  }

  void enableAndSetAttribPointer(GLint location, GLint size, GLenum type, GLboolean normalize, GLsizei stride, size_t offset) {
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, size, type, normalize, stride, (void*) offset);
  }

  void enableAndSetAttribIPointer(GLint location, GLint size, GLenum type, GLsizei stride, size_t offset) {
    glEnableVertexAttribArray(location);
    glVertexAttribIPointer(location, size, type, stride, (void*) offset);
  }
};

#endif
