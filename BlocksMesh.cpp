#include "Block.hpp"
#include "BlocksMesh.hpp"

BlocksMesh BlocksMesh::buildFromBlocksMap(const BlocksMap& blocksMap) {
  const Block* displayedBlock = &*blocksMap.at(0, 0, 0);

  BlocksMesh blocksMesh;

  blocksMesh.vertices = {
    // +X
    { 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.f, 0.f},
    { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.f, 0.f},
    { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.f, 0.f},
    { 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.f, 0.f},
    // -X
    {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.f, 0.f},
    {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.f, 0.f},
    {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.f, 0.f},
    {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.f, 0.f},
    // +Y
    {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.f, 0.f},
    { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.f, 0.f},
    {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.f, 0.f},
    { 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.f, 0.f},
    // -Y
    { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.f, 0.f},
    {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.f, 0.f},
    { 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.f, 0.f},
    {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.f, 0.f},
    // +Z
    {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.f, 0.f},
    { 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.f, 0.f},
    {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.f, 0.f},
    { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.f, 0.f},
    // -Z
    { 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f, 0.f},
    {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f, 0.f},
    { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f, 0.f},
    {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f, 0.f},
  };
  for (size_t i = 0; i < displayedBlock->block_type->vertex_texture_coordinates.size(); i++) {
    blocksMesh.vertices[i].u = displayedBlock->block_type->vertex_texture_coordinates[i][0];
    blocksMesh.vertices[i].v = displayedBlock->block_type->vertex_texture_coordinates[i][1];
  }

  blocksMesh.vertexIndices.reserve(blocksMesh.vertices.size() / 4 * 6);
  for (size_t i = 0; i < blocksMesh.vertices.size() / 4; i++) {
    blocksMesh.vertexIndices.push_back(i * 4 + 0);
    blocksMesh.vertexIndices.push_back(i * 4 + 1);
    blocksMesh.vertexIndices.push_back(i * 4 + 2);
    blocksMesh.vertexIndices.push_back(i * 4 + 1);
    blocksMesh.vertexIndices.push_back(i * 4 + 3);
    blocksMesh.vertexIndices.push_back(i * 4 + 2);
  }

  return blocksMesh;
}
