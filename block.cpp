#include <iostream>
#include <sstream>
#include <openssl/sha.h>
#include <string>
#include <ctime>
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
void Block::print(){

	std::time_t t = timestamp;
	std::tm tm = *std::localtime(&t);

	std::stringstream ss;
	ss << "\n| prevBlockHash: " << prevBlockHash << " |\n| timestamp: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " |\n| nonce: " << nonce << " |\n";
	std::cout << ss.str() << std::endl;

	for (Transaction tx : transactions){
		tx.print();
	}
}

std::string Block::sha256(std::string is){

	unsigned char hash[32];
	SHA256(reinterpret_cast<const unsigned char *>(is.c_str()), is.size(), hash);

	std::stringstream os;
	for (int i = 0; i < 32; i++){
		os << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
	}
	return os.str();
	
}

std::string Block::calculateHash(){
	std::string blockstring = serializeBlock();
	return sha256(blockstring);
	
}
/*void Block::mineBlock(){
	int difficulty = 5;
	nonce = 0;
	std::string thisBlockHash = calculateHash();
	while (thisBlockHash.substr(0, difficulty) != std::string(difficulty, '0')){
		nonce++;
		thisBlockHash = calculateHash();
	}
}*/
std::string Block::calculateMerkleRoot(){
	std::vector<std::string> hashes;
	std::string merkleRoot;

	for (Transaction& tx : transactions){
		hashes.push_back(tx.calculateHash());
	}

	if (hashes.empty()) return "";

	while (hashes.size() > 1){
		std::vector<std::string> newLevel;
		for (int i = 0; i < hashes.size(); i += 2){
			if (i < hashes.size() - 1){
				newLevel.push_back(sha256(hashes[i] + hashes[i+1]));
			} else {
				newLevel.push_back(sha256(hashes[i] + hashes[i]));
			}
		}
			hashes = std::move(newLevel);
		}
	return hashes[0];
}
