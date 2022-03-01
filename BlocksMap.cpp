#include <stdexcept>
#include "BlocksMap.hpp"

BlocksMap::BlocksMap(glm::ivec3 basePosition_, glm::ivec3 size_) {
  basePosition = basePosition_;
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

const Block* BlocksMap::get(glm::ivec3 position) const {
  std::optional<size_t> storageLocation = calculateStorageLocation(position);
  if (storageLocation) {
    const std::optional<Block>& optionalBlock = storage[*storageLocation];
    if (optionalBlock) {
      return &*optionalBlock;
    }
  }
  return nullptr;
}

std::optional<size_t> BlocksMap::calculateStorageLocation(glm::ivec3 position) const {
  glm::ivec3 internalPosition = position - basePosition;
  if (
    internalPosition.x >= 0 && internalPosition.x < size.x &&
    internalPosition.y >= 0 && internalPosition.y < size.y &&
    internalPosition.z >= 0 && internalPosition.z < size.z
  ) {
    return internalPosition.y * size.x * size.z + internalPosition.z * size.x + internalPosition.x;
  } else {
    return {};
  }
}

glm::ivec3 BlocksMap::calculatePosition(size_t storageLocation) const {
  glm::ivec3 internalPosition;
  internalPosition.y = storageLocation / (size.x * size.z);
  internalPosition.z = (storageLocation % (size.x * size.z)) / size.x;
  internalPosition.x = storageLocation % size.x;
  return internalPosition + basePosition;
}
