#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_
#include "BlockType.hpp"

// A block on the map
class Block {
private:
  BlockType& _blockType;

public:
  Block(BlockType& blockType_) : _blockType(blockType_) {}

  const BlockType& blockType() const { return _blockType; }
};

#endif
