#pragma once
#include <ctime>
#include <string>
#include <vector>
#include "transaction.h"

class Block {
public:
	std::string prevBlockHash;
	time_t timestamp;
	long long nonce;
	std::string MerkleRoot;
	std::vector<Transaction> transactions;

	Block();
	Block(std::string prevBlockHash);
	
	std::string serializeBlock();
	void print();

	// helper function to hash strings
	std::string sha256(std::string is);
	// serialize and hash
	std::string calculateHash();
	std::string calculateMerkleRoot();

	void mineBlock();

};

