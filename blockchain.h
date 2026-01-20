#pragma once
#include <vector>
#include "block.h"

class Blockchain {
public:
	std::vector<Block> chain;

	Blockchain();
	// creates genesis block with a transaction to the foundation wallet
	void createGenesisBlock();
	Block& lastBlock();
	void addBlock(Block& block);
	bool isEmpty();
	void print();
};

