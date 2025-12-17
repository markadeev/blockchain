#pragma once
#include <vector>
#include "block.h"

class Blockchain {
public:
	std::vector<Block> chain;

	Blockchain();
	void createGenesisBlock();
	Block& lastBlock();
	void addBlock(Block& block);
	bool isEmpty();
};

