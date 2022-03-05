#ifndef _GL_BUFFER__HPP_
#define _GL_BUFFER__HPP_
#include <GL/glew.h>

class GLBuffer {
private:
  GLuint _id;
  GLenum _type;

public:
  GLBuffer(GLenum type_) : _type(type_) {
    glGenBuffers(1, &_id);
  }
  ~GLBuffer() {
    glDeleteBuffers(1, &_id);
  }

  GLBuffer(const GLBuffer&) = delete;
  GLBuffer& operator=(const GLBuffer&) = delete;

  GLuint id() const { return _id; }

  void bind() {
    glBindBuffer(_type, _id);
  }

  template <typename C>
  void sendData(const C& dataContainer, GLenum usage) {
    glBufferData(_type, dataContainer.size() * sizeof(typename C::value_type), dataContainer.data(), usage);
  }

  void sendData(size_t size, GLenum usage) {
    glBufferData(_type, size, nullptr, usage);
  }
};

#endif
