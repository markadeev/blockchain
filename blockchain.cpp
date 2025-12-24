#include <iostream>
#include <vector>
//#include "block.h"
#include "blockchain.h"

Blockchain::Blockchain()
	:chain({})
{
	createGenesisBlock();
}

void Blockchain::createGenesisBlock(){
	Block genesisBlock;
	chain.push_back(genesisBlock);
}
Block& Blockchain::lastBlock(){
	return chain.back();
}
void Blockchain::addBlock(Block& block){
	chain.push_back(block);
}
bool Blockchain::isEmpty(){
	return chain.empty();	
}
void Blockchain::print(){
	for (int i = 0; i < chain.size() ; i++){
		std::cout << "Block " << i << std::endl;
		chain[i].print();
	}
}

