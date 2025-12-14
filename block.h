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
	std::string calculateHash();
	std::string calculateMerkleRoot();
	std::string sha256(std::string is);

	void mineBlock();

};

