#include <algorithm>
#include "Block.hpp"
#include "BlocksMesh.hpp"

BlocksMesh BlocksMesh::buildFromBlocksMap(const BlocksMap& blocksMap) {
  BlocksMesh blocksMesh;

  for (size_t i = 0; i < blocksMap.storage.size(); i++) {
    glm::ivec3 position = blocksMap.calculatePosition(i);
    const std::optional<Block>& block = blocksMap.storage[i];
    if (!block) continue;

    // Add the vertices of exposed faces to the mesh
    for (const BlockFaceDefinition& face : block->blockType().faces()) {
      // Determine whether this face is at or beyond the boundary of the block, and in which direction
      std::optional<glm::ivec3> faceDirection;
      if (std::all_of(face.vertices().begin(), face.vertices().end(), [] (const BlockVertex& vertex) { return vertex.x >= 0.5f; })) {
        faceDirection.emplace(glm::ivec3(1, 0, 0));
      } else if (std::all_of(face.vertices().begin(), face.vertices().end(), [] (const BlockVertex& vertex) { return vertex.x <= -0.5f; })) {
        faceDirection.emplace(glm::ivec3(-1, 0, 0));
      } else if (std::all_of(face.vertices().begin(), face.vertices().end(), [] (const BlockVertex& vertex) { return vertex.y >= 0.5f; })) {
        faceDirection.emplace(glm::ivec3(0, 1, 0));
      } else if (std::all_of(face.vertices().begin(), face.vertices().end(), [] (const BlockVertex& vertex) { return vertex.y <= -0.5f; })) {
        faceDirection.emplace(glm::ivec3(0, -1, 0));
      } else if (std::all_of(face.vertices().begin(), face.vertices().end(), [] (const BlockVertex& vertex) { return vertex.z >= 0.5f; })) {
        faceDirection.emplace(glm::ivec3(0, 0, 1));
      } else if (std::all_of(face.vertices().begin(), face.vertices().end(), [] (const BlockVertex& vertex) { return vertex.z <= -0.5f; })) {
        faceDirection.emplace(glm::ivec3(0, 0, -1));
      }
      if (faceDirection) {
        const Block* adjacentBlock = blocksMap.get(position + *faceDirection);
        // Discard faces with non-transparent adjacent block
        if (adjacentBlock && !adjacentBlock->blockType().attributes().transparent) continue;
      }

      // Add vertices to mesh
      blocksMesh.vertices.reserve(blocksMesh.vertices.size() + face.vertices().size());
      for (const BlockVertex& definedVertex : face.vertices()) {
        BlockVertex vertex = definedVertex;
        // Move vertex position with respect to block position
        vertex.x += position.x;
        vertex.y += position.y;
        vertex.z += position.z;

        blocksMesh.vertices.push_back(vertex);
      }

      // Add vertex indices to mesh
      blocksMesh.vertexIndices.reserve(face.vertexIndices().size());
      for (GLuint definedVertexIndex : face.vertexIndices()) {
        blocksMesh.vertexIndices.push_back(blocksMesh.vertices.size() - face.vertices().size() + definedVertexIndex);
      }
    }
  }

  return blocksMesh;
}
