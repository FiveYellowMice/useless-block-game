#ifndef _BLOCKS_MESH_HPP_
#define _BLOCKS_MESH_HPP_
#include <vector>
#include <array>
#include <GL/glew.h>
#include "Vertex.hpp"
#include "BlocksMap.hpp"

class BlocksMesh {
public:
  std::vector<Vertex> vertices;
  std::vector<GLuint> vertexIndices;

  static BlocksMesh buildFromBlocksMap(const BlocksMap& blocksMap);
};

#endif
