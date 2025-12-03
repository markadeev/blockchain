#pragma once
#include "node.h"
#include "block.h"

class Miner: public Node {

	Block createBlock();
	Block mineBlock();

};
