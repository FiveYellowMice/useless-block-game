#include <stdexcept>
#include "BlocksMap.hpp"

BlocksMap::BlocksMap(glm::ivec3 size_) {
  size = size_;
  storage.resize(size.x * size.y * size.z);
}

std::optional<Block>& BlocksMap::operator[](glm::ivec3 position) {
  std::optional<size_t> storageLocation = calculateStorageLocation(position);
  if (storageLocation) {
    return storage[*storageLocation];
  } else {
    throw std::out_of_range("position is outside of the BlocksMap");
  }
}

const std::optional<Block>& BlocksMap::operator[](glm::ivec3 position) const {
  std::optional<size_t> storageLocation = calculateStorageLocation(position);
  if (storageLocation) {
    return storage[*storageLocation];
  } else {
    throw std::out_of_range("position is outside of the BlocksMap");
  }
}

const std::optional<Block> BlocksMap::get(glm::ivec3 position) const {
  std::optional<size_t> storageLocation = calculateStorageLocation(position);
  if (storageLocation) {
    return storage[*storageLocation];
  } else {
    return {};
  }
}

std::optional<size_t> BlocksMap::calculateStorageLocation(glm::ivec3 position) const {
  if (
    position.x >= 0 && position.x < size.x &&
    position.y >= 0 && position.y < size.y &&
    position.z >= 0 && position.z < size.z
  ) {
    return position.y * size.x * size.z + position.z * size.x + position.x;
  } else {
    return {};
  }
}

glm::ivec3 BlocksMap::calculatePosition(size_t storageLocation) const {
  glm::ivec3 position;
  position.y = storageLocation / (size.x * size.z);
  position.z = (storageLocation % (size.x * size.z)) / size.x;
  position.x = storageLocation % size.x;
  return position;
}
