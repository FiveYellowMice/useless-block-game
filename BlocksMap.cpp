#include "BlocksMap.hpp"

BlocksMap::BlocksMap(size_t x_size_, size_t y_size_, size_t z_size_) {
  x_size = x_size_;
  y_size = y_size_;
  z_size = z_size_;
  storage.resize(x_size * y_size * z_size);
}

std::optional<Block>& BlocksMap::at(size_t x, size_t y, size_t z) {
  return storage[calculateStorageLocation(x, y, z)];
}

const std::optional<Block>& BlocksMap::at(size_t x, size_t y, size_t z) const {
  return storage[calculateStorageLocation(x, y, z)];
}

size_t BlocksMap::calculateStorageLocation(size_t x, size_t y, size_t z) const {
  return y * x_size * z_size + z * x_size + x;
}
