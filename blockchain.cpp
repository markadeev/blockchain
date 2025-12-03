#include <iostream>
#include <vector>
//#include "block.h"
#include "blockchain.h"

void Blockchain::createGenesisBlock(){
	Block genesisBlock("0");
	chain.push_back(genesisBlock);
}
Block& Blockchain::getLatestBlock(){
	return chain.back();
}
void Blockchain::addBlock(Block newBlock){
	chain.push_back(newBlock);
}
