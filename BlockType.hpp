#ifndef _BLOCK_TYPE_HPP_
#define _BLOCK_TYPE_HPP_
#include <string>
#include <array>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include "StreamingTextures.hpp"

struct BlockVertex {
  float x, y, z;
  float nx, ny, nz;
  float u, v;
  GLuint tx, ty;
};

// Definition of each face on the block
class BlockFaceDefinition {
private:
  std::vector<BlockVertex> _vertices;
  std::vector<GLuint> _vertexIndices;
  std::shared_ptr<StreamingTexturesPart> _texturePartPtr;

public:
  BlockFaceDefinition(std::vector<BlockVertex>&& vertices_, std::vector<GLuint>&& vertexIndices_, std::shared_ptr<StreamingTexturesPart> texturePartPtr_);

  const std::vector<BlockVertex>& vertices() const { return _vertices; }
  const std::vector<GLuint>& vertexIndices() const { return _vertexIndices; }
  std::shared_ptr<StreamingTexturesPart> texturePartPtr() const { return _texturePartPtr; }
};

struct BlockTypeAttributes {
  bool transparent; // there are transparent pixels in the texture
};

// A type of block that could exist in the game
class BlockType {
private:
  std::string _blockId;
  BlockTypeAttributes _attributes;
  std::vector<BlockFaceDefinition> _faces;

public:
  BlockType(std::string&& blockId_, BlockTypeAttributes&& attributes_, std::vector<BlockFaceDefinition>&& faces_);
  // Convenience function of defining a 6-faced full block
  BlockType(std::string&& blockId_, BlockTypeAttributes&& attributes_, std::array<std::shared_ptr<StreamingTexturesPart>, 6>&& faceTextures);

  const std::string& blockId() const { return _blockId; }
  const BlockTypeAttributes& attributes() const { return _attributes; }
  const std::vector<BlockFaceDefinition>& faces() const { return _faces; };
};

#endif
