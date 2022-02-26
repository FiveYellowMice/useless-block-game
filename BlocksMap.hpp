#ifndef _BLOCKS_MAP_HPP_
#define _BLOCKS_MAP_HPP_
#include <vector>
#include <optional>
#include "Block.hpp"

class BlocksMap {
public:
  size_t x_size;
  size_t y_size;
  size_t z_size;
  std::vector<std::optional<Block>> storage;

  BlocksMap(size_t x_size_, size_t y_size_, size_t z_size_);

  std::optional<Block>& at(size_t x, size_t y, size_t z);
  const std::optional<Block>& at(size_t x, size_t y, size_t z) const;

private:
  size_t calculateStorageLocation(size_t x, size_t y, size_t z) const;
};

#endif
