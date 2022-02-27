#include <GL/glew.h>
#include "BlockType.hpp"

BlockFaceDefinition::BlockFaceDefinition(std::vector<Vertex>&& vertices_, std::vector<GLuint>&& vertexIndices_, std::shared_ptr<StreamingTexturesPart> texturePartPtr_) {
  _vertices = vertices_;
  _vertexIndices = vertexIndices_;
  _texturePartPtr = texturePartPtr_;

  // Adjust texture coordinates according to the StreamingTexturesPart
  for (Vertex& vertex : _vertices) {
    vertex.u = _texturePartPtr->scaleU(vertex.u);
    vertex.v = _texturePartPtr->scaleV(vertex.v);
  }
}

BlockType::BlockType(std::string&& blockId_, std::vector<BlockFaceDefinition>&& faces_) {
  _blockId = blockId_;
  _faces = faces_;
}

BlockType::BlockType(std::string&& blockId_, std::array<std::shared_ptr<StreamingTexturesPart>, 6>&& faceTextures) {
  // faceTextures represent the 6 faces of a full block, order:
  // X+, X-, Y+, Y-, Z+, Z-

  _blockId = blockId_;
  _faces = std::vector<BlockFaceDefinition>();
  _faces.reserve(6);

  const std::array<std::array<Vertex, 4>, 6> predefinedFullBlockFaceVertices = {{
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
    std::vector<Vertex> faceVertices;
    faceVertices.reserve(4);
    for (const Vertex& predefinedFaceVertex : predefinedFullBlockFaceVertices[i]) {
      faceVertices.push_back(predefinedFaceVertex);
    }
    _faces.push_back(BlockFaceDefinition(std::move(faceVertices), std::vector<GLuint>{0, 1, 2, 1, 3, 2}, faceTextures[i]));
  }
}

// BlockType::BlockType(std::string&& block_id_, std::array<std::array<float, 2>, 24>&& vertex_texture_coordinates_) {
//   block_id = block_id_;
//   vertex_texture_coordinates = vertex_texture_coordinates_;
// 
//   texture = std::make_shared<Texture>(GL_TEXTURE_2D);
//   texture->loadFromFile("textures/" + block_id + ".png");
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// }
