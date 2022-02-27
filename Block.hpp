#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_
#include "BlockType.hpp"

// A block on the map
class Block {
public:
  BlockType* blockType;

  Block(BlockType* blockType_);
};

#endif
