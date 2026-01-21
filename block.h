#pragma once
#include <ctime>
#include <string>
#include <vector>
#include "transaction.h"

class Block {
public:
	// hash of the previous block in the chain
	std::string prevBlockHash;
	// timestamp at the time of block generation
	time_t timestamp;
	// a number that is used to generate block's hash with the
	// appropriate MINING_DIFFICULTY length of leading zeros
	long long nonce;
	// hashes of block transactions "combined" into a single hash
	std::string MerkleRoot;
	// vector to store block's transactions
	std::vector<Transaction> transactions;

	// constructor for a block with prevBlockHash "0" and nonce 0
	Block();
	// constructor for a block with a given prevBlockHash and nonce 0 
	Block(std::string prevBlockHash);
	
	// serialize block header and transactions into a single string
	std::string serializeBlock();
	void print();

	// helper function for calculateHash() to hash strings
	std::string sha256(std::string is);
	// serialize block and hash
	std::string calculateHash();
	// calculate hash for each transaction
	// add each of the two neighboring transaction hashes and hash the result
	// repeat for each level until one hash remains
	std::string calculateMerkleRoot();


};

