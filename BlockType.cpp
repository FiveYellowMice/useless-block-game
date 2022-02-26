#include <GL/glew.h>
#include "BlockType.hpp"

BlockType::BlockType(std::string&& block_id_, std::array<std::array<float, 2>, 24>&& vertex_texture_coordinates_) {
  block_id = block_id_;
  vertex_texture_coordinates = vertex_texture_coordinates_;

  texture = std::make_shared<Texture>(GL_TEXTURE_2D);
  texture->loadFromFile("textures/" + block_id + ".png");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
