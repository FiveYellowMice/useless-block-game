#ifndef _STREAMING_TEXTURES_HPP_
#define _STREAMING_TEXTURES_HPP_
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <stdexcept>
#include <GL/glew.h>

class StreamingTexturesPart;

// Manages a set of parallel streaming textures, containing square cells of size cellSideLength^2, layed out in a square grid patterrn, each texture has a total of cellCountPerSide^2 cells
class StreamingTextures {
private:
  size_t _cellSideLength;
  size_t _cellCountPerSide;

  std::vector<GLuint> _textureIds;
  std::vector<GLenum> _textureFormats;

  std::vector<bool> _registry; // size is cellCount^2, count from bottom-left, row major

public:
  StreamingTextures(size_t cellSideLength_, size_t cellCountPerSide_, std::vector<GLenum>&& textureFormats_, std::function<void(size_t, GLuint)> configFunc = {});
  ~StreamingTextures();

  StreamingTextures(const StreamingTextures&) = delete;
  StreamingTextures& operator=(const StreamingTextures&) = delete;

  size_t cellSideLength() const { return _cellSideLength; }
  size_t cellCountPerSide() const { return _cellCountPerSide; }
  const std::vector<GLuint>& textureIds() const { return _textureIds; }
  const std::vector<GLenum>& textureFormats() const { return _textureFormats; }

  // Bind the textures to texture image units 0, 1, 2 ...
  void bind();
  // Allocate a new cell in the grid to store new data
  std::shared_ptr<StreamingTexturesPart> allocate(const std::vector<std::vector<uint8_t>>& data);
  std::shared_ptr<StreamingTexturesPart> allocateFromFiles(const std::vector<std::string>& filenames);

  static GLenum sizedInternalFormatToBaseInternalFormat(GLenum sizedInternalFormat);
  static size_t sizedInternalFormatToPixelSize(GLenum sizedInternalFormat);

  friend class StreamingTexturesPart;

  class AllocationError : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };
};

// Represents to a cell in the StreamingTextures grid
class StreamingTexturesPart {
private:
  StreamingTextures& _manager;
  size_t _xLocation;
  size_t _yLocation;

  // Do not allow an instance to be created freely
  StreamingTexturesPart(StreamingTextures& manager_, size_t xLocation_, size_t yLocation_) : _manager(manager_), _xLocation(xLocation_), _yLocation(yLocation_) {}

public:
  StreamingTexturesPart(const StreamingTexturesPart&) = delete;
  StreamingTexturesPart& operator=(const StreamingTexturesPart&) = delete;

  ~StreamingTexturesPart() {
    // Remove itself from the manager
    _manager._registry[_yLocation * _manager._cellCountPerSide + _xLocation] = false;
  }

  StreamingTextures& manager() const { return _manager; }
  size_t xLocation() const { return _xLocation; }
  size_t yLocation() const { return _yLocation; }

  friend class StreamingTextures;
};

#endif
