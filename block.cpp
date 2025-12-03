#include <sstream>
#include <openssl/sha.h>
#include <string>
#include <iomanip>
#include "block.h"

// constructor
Block::Block()
	:prevBlockHash(""),
	timestamp(std::time(nullptr)),
	nonce(0),
	MerkleRoot(""),
	transactions({})
{
	//optional mineBlock function
}

Block::Block(std::string prevBlockHash)
	:prevBlockHash(prevBlockHash),
	timestamp(std::time(nullptr)),
	nonce(0),
	MerkleRoot(""),
	transactions({})
{
	//optional mineBlock function
}
std::string Block::serializeBlock(){
	std::stringstream ss;
	ss << prevBlockHash << " | " << timestamp << " | " << nonce << " | ";
	for (Transaction tx : transactions){
		ss << tx.serializeTransaction();
	}
	return ss.str();
}

std::string Block::calculateHash(){

	std::string blockstring = serializeBlock();

	unsigned char hash[32];
	SHA256(reinterpret_cast<const unsigned char *>(blockstring.c_str()), blockstring.size(), hash);

	std::stringstream os;
	for (int i = 0; i < 32; i++){
		os << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
	}
	return os.str();
	
}
void Block::mineBlock(){
	int difficulty = 5;
	nonce = 0;
	while (thisBlockHash.substr(0, difficulty) != std::string(difficulty, '0')){
		nonce++;
		thisBlockHash = calculateHash();
	}
}
