#include <GL/glew.h>
#include "BlockType.hpp"

BlockFaceDefinition::BlockFaceDefinition(std::vector<BlockVertex>&& vertices_, std::vector<GLuint>&& vertexIndices_, std::shared_ptr<StreamingTexturesPart> texturePartPtr_) {
  _vertices = vertices_;
  _vertexIndices = vertexIndices_;
  _texturePartPtr = texturePartPtr_;

  // Allow the shader to adjust texture coordinates according to the StreamingTexturesPart
  for (BlockVertex& vertex : _vertices) {
    vertex.tx = _texturePartPtr->xLocation();
    vertex.ty = _texturePartPtr->yLocation();
  }
}

BlockType::BlockType(std::string&& blockId_, BlockTypeAttributes&& attributes_, std::vector<BlockFaceDefinition>&& faces_) {
  _blockId = blockId_;
  _attributes = attributes_;
  _faces = faces_;
}

BlockType::BlockType(std::string&& blockId_, BlockTypeAttributes&& attributes_, std::array<std::shared_ptr<StreamingTexturesPart>, 6>&& faceTextures) {
  // faceTextures represent the 6 faces of a full block, order:
  // X+, X-, Y+, Y-, Z+, Z-

  _blockId = blockId_;
  _attributes = attributes_;
  _faces = std::vector<BlockFaceDefinition>();
  _faces.reserve(6);

  const std::array<std::array<BlockVertex, 4>, 6> predefinedFullBlockFaceVertices = {{
    // X+
    {{
      { 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.f, 0.f},
      { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.f, 0.f},
      { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.f, 1.f},
      { 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.f, 1.f},
    }},
    // X-
    {{
      {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.f, 0.f},
      {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.f, 0.f},
      {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.f, 1.f},
      {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.f, 1.f},
    }},
    // Y+
    {{
      {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.f, 0.f},
      { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.f, 0.f},
      {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.f, 1.f},
      { 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.f, 1.f},
    }},
    // Y-
    {{
      { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.f, 0.f},
      {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.f, 0.f},
      { 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.f, 1.f},
      {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.f, 1.f},
    }},
    // Z+
    {{
      {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.f, 0.f},
      { 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.f, 0.f},
      {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.f, 1.f},
      { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.f, 1.f},
    }},
    // Z-
    {{
      { 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f, 0.f},
      {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.f, 0.f},
      { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f, 1.f},
      {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.f, 1.f},
    }},
  }};

  for (size_t i = 0; i < predefinedFullBlockFaceVertices.size(); i++) {
    std::vector<BlockVertex> faceVertices;
    faceVertices.reserve(4);
    for (const BlockVertex& predefinedFaceVertex : predefinedFullBlockFaceVertices[i]) {
      faceVertices.push_back(predefinedFaceVertex);
    }
    _faces.push_back(BlockFaceDefinition(std::move(faceVertices), std::vector<GLuint>{0, 1, 2, 1, 3, 2}, faceTextures[i]));
  }
}
