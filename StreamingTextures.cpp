#include <optional>
#include <GL/glew.h>
#include "load_png.hpp"
#include "StreamingTextures.hpp"

StreamingTextures::StreamingTextures(size_t cellSideLength_, size_t cellCountPerSide_, std::vector<GLenum>&& textureFormats_, std::function<void(size_t, GLuint)> configFunc) {
  _cellSideLength = cellSideLength_;
  _cellCountPerSide = cellCountPerSide_;
  _textureFormats = textureFormats_;

  // Make sure there is no invalid texture format
  for (GLenum sizedInternalFormat : _textureFormats) {
    sizedInternalFormatToBaseInternalFormat(sizedInternalFormat);
  }

  _registry.resize(_cellCountPerSide * _cellCountPerSide, false);

  _textureIds.resize(_textureFormats.size());
  glGenTextures(_textureIds.size(), _textureIds.data());

  for (size_t i = 0; i < _textureIds.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, _textureIds[i]);
    glTexStorage2D(GL_TEXTURE_2D, 1, _textureFormats[i], _cellSideLength * _cellCountPerSide, _cellSideLength * _cellCountPerSide);
    configFunc(i, _textureIds[i]);
  }
}

StreamingTextures::~StreamingTextures() {
  glDeleteTextures(_textureIds.size(), _textureIds.data());
}

void StreamingTextures::bind() {
  for (size_t i = 0; i < _textureIds.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, _textureIds[i]);
  }
}

std::shared_ptr<StreamingTexturesPart> StreamingTextures::allocate(const std::vector<std::vector<uint8_t>>& data) {
  if (data.size() != _textureIds.size()) {
    throw std::invalid_argument("number of data is inconsistent with number of textures");
  }

  // Find an empty spot in the registry
  std::optional<size_t> emptySpotIndex;
  for (size_t i = 0; i < _registry.size(); i++) {
    if (_registry[i] == false) {
      _registry[i] = true;
      emptySpotIndex.emplace(i);
      break;
    }
  }
  if (!emptySpotIndex) {
    throw AllocationError("this StreamingTextures is full");
  }
  size_t xLocation = *emptySpotIndex % _cellCountPerSide;
  size_t yLocation = *emptySpotIndex / _cellCountPerSide;
  size_t xOffset = xLocation * _cellSideLength;
  size_t yOffset = yLocation * _cellSideLength;

  // Store texture data into the designated area
  for(size_t i = 0; i < _textureIds.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, _textureIds[i]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, _cellSideLength, _cellSideLength, sizedInternalFormatToBaseInternalFormat(_textureFormats[i]), GL_UNSIGNED_BYTE, data[i].data());
  }

  return std::shared_ptr<StreamingTexturesPart>(new StreamingTexturesPart(*this, xLocation, yLocation));
}

std::shared_ptr<StreamingTexturesPart> StreamingTextures::allocateFromFiles(const std::vector<std::string>& filenames) {
  if (filenames.size() != _textureIds.size()) {
    throw std::invalid_argument("number of data is inconsistent with number of textures");
  }

  std::vector<std::vector<uint8_t>> data(_textureIds.size());

  for (size_t i = 0; i < _textureIds.size(); i++) {
    data[i].resize(_cellSideLength * _cellSideLength * sizedInternalFormatToPixelSize(_textureFormats[i]));

    load_png(filenames[i].c_str(), _textureFormats[i], _cellSideLength, _cellSideLength, data[i].data());
  }

  return allocate(data);
}

GLenum StreamingTextures::sizedInternalFormatToBaseInternalFormat(GLenum sizedInternalFormat) {
  switch (sizedInternalFormat) {
    case GL_R8:
      return GL_RED;
    case GL_RG8:
      return GL_RG;
    case GL_RGB8:
      return GL_RGB;
    case GL_RGBA8:
      return GL_RGBA;
    default:
      throw std::invalid_argument("unsupported texture format");
  }
}

size_t StreamingTextures::sizedInternalFormatToPixelSize(GLenum sizedInternalFormat) {
  switch (sizedInternalFormat) {
    case GL_R8:
      return 1;
    case GL_RG8:
      return 2;
    case GL_RGB8:
      return 3;
    case GL_RGBA8:
      return 4;
    default:
      throw std::invalid_argument("unsupported texture format");
  }
}
