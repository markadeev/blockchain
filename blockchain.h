#pragma once
#include <vector>
#include "block.h"

class Blockchain {
public:

	// constructor for a blockchain with genesis block
	// which includes a transaction with FOUNDATION_AMOUNT of coins
	// to the FOUNDATION_PUBKEY
	// all nodes are initialized with such genesis block
	Blockchain();
	// helper function for Blockchain() constructor
	// create genesis block with a transaction to the foundation wallet
	void createGenesisBlock();

	// since chain is private, helper functions below are used to work with it
	// return the last block from the chain 
	Block& lastBlock();
	// add block to the end of the chain
	void addBlock(Block& block);
	// check if the chain is empty
	bool isEmpty();
	// return chain size
	int chainSize();
	void print();

private:
	// vector to store blocks of the blockchain
	std::vector<Block> chain;
};

