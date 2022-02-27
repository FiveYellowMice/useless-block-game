#ifndef _BLOCKS_MAP_HPP_
#define _BLOCKS_MAP_HPP_
#include <vector>
#include <optional>
#include <algorithm>
#include <glm/glm.hpp>
#include "Block.hpp"

class BlocksMap {
public:
  glm::ivec3 size;
  std::vector<std::optional<Block>> storage;

  BlocksMap(glm::ivec3 size_);

  std::optional<Block>& operator[](glm::ivec3 position);
  const std::optional<Block>& operator[](glm::ivec3 position) const;

  const std::optional<Block> get(glm::ivec3 position) const;

  std::optional<size_t> calculateStorageLocation(glm::ivec3 position) const;
  glm::ivec3 calculatePosition(size_t storageLocation) const;
};

#endif
