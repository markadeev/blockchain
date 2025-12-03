#pragma once
#include "node.h"
#include "block.h"

class Miner: public Node {
public:

	Block buildBlock();
	Block mineBlock(Block& block);

};
