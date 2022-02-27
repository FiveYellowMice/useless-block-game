#include <tuple>
#include "Block.hpp"
#include "BlocksMesh.hpp"

BlocksMesh BlocksMesh::buildFromBlocksMap(const BlocksMap& blocksMap) {
  BlocksMesh blocksMesh;

  for (size_t i = 0; i < blocksMap.storage.size(); i++) {
    glm::ivec3 position = blocksMap.calculatePosition(i);
    std::optional<Block> block = blocksMap.storage[i];
    if (!block) continue;

    // The faces and their vertices a block should have
    const std::array<std::tuple<glm::ivec3, std::array<Vertex, 4>>, 6> blockFacesAndVertices {
      // +X
      std::tuple<glm::ivec3, std::array<Vertex, 4>>{
        glm::ivec3(1, 0, 0), std::array<Vertex, 4>{{
          { 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.f, 0.f},
          { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.f, 0.f},
          { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.f, 0.f},
          { 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.f, 0.f},
        }}
      },
      // -X
      std::tuple<glm::ivec3, std::array<Vertex, 4>>{
        glm::ivec3(-1, 0, 0), std::array<Vertex, 4>{{
          {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.f, 0.f},
          {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.f, 0.f},
          {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.f, 0.f},
          {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.f, 0.f},
        }}
      },
      // +Y
      std::tuple<glm::ivec3, std::array<Vertex, 4>>{
        glm::ivec3(0, 1, 0), std::array<Vertex, 4>{{
          {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.f, 0.f},
          { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.f, 0.f},
          {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.f, 0.f},
          { 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.f, 0.f},
        }}
      },
      // -Y
      std::tuple<glm::ivec3, std::array<Vertex, 4>>{
        glm::ivec3(0, -1, 0), std::array<Vertex, 4>{{
          { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.f, 0.f},
          {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.f, 0.f},
          { 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.f, 0.f},
          {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.f, 0.f},
        }}
      },
      // +Z
      std::tuple<glm::ivec3, std::array<Vertex, 4>>{
        glm::ivec3(0, 0, 1), std::array<Vertex, 4>{{
          {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.f, 0.f},
          { 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.f, 0.f},
          {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.f, 0.f},
          { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.f, 0.f},
        }}
      },
      // -Z
      std::tuple<glm::ivec3, std::array<Vertex, 4>>{
        glm::ivec3(0, 0, -1), std::array<Vertex, 4>{{
          { 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f, 0.f},
          {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f, 0.f},
          { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f, 0.f},
          {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f, 0.f},
        }}
      },
    };

    // Add the vertices of exposed faces to the mesh
    for (size_t j = 0; j < blockFacesAndVertices.size(); j++) {
      auto [faceDirection, faceVertices] = blockFacesAndVertices[j];

      // Discard faces with adjacent block
      std::optional<Block> adjacentBlock = blocksMap.get(position + faceDirection);
      if (adjacentBlock) continue;

      for (size_t k = 0; k < 4; k++) {
        // Move vertex position with respect to block position
        faceVertices[k].x += position.x;
        faceVertices[k].y += position.y;
        faceVertices[k].z += position.z;
        // Set texture coordinates
        faceVertices[k].u = block->block_type->vertex_texture_coordinates[j * 4 + k][0];
        faceVertices[k].v = block->block_type->vertex_texture_coordinates[j * 4 + k][1];
      }

      // Add vertices and vertex indices to mesh
      blocksMesh.vertices.insert(blocksMesh.vertices.end(), faceVertices.begin(), faceVertices.end());
      blocksMesh.vertexIndices.push_back(blocksMesh.vertices.size() - 4 + 0);
      blocksMesh.vertexIndices.push_back(blocksMesh.vertices.size() - 4 + 1);
      blocksMesh.vertexIndices.push_back(blocksMesh.vertices.size() - 4 + 2);
      blocksMesh.vertexIndices.push_back(blocksMesh.vertices.size() - 4 + 1);
      blocksMesh.vertexIndices.push_back(blocksMesh.vertices.size() - 4 + 3);
      blocksMesh.vertexIndices.push_back(blocksMesh.vertices.size() - 4 + 2);
    }
  }

  return blocksMesh;
}
