#ifndef _BLOCK_TYPE_HPP_
#define _BLOCK_TYPE_HPP_
#include <string>
#include <array>
#include <memory>
#include "Texture.hpp"

// A type of block that could exist in the game
class BlockType {
public:
  std::string block_id;
  std::shared_ptr<Texture> texture;
  std::array<std::array<float, 2>, 24> vertex_texture_coordinates;

  BlockType(std::string&& block_id_, std::array<std::array<float, 2>, 24>&& vertex_texture_coordinates_);
};

#endif
