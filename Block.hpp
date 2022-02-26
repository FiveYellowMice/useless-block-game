#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_
#include "BlockType.hpp"

// A block on the map
class Block {
public:
  BlockType* block_type;

  Block(BlockType* block_type_);
};

#endif
