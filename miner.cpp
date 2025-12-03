#include "miner.h"

Block Miner::buildBlock(){
	Block block;
	block.prevBlockHash = chain.back().calculateHash();
	int n = 0;
	for (Transaction tx : mempool){
		if (n == 10) break;
		block.transactions.push_back(tx);
	}	
	return block;
}
Block Miner::mineBlock(Block& block){

	int difficulty = 4;

	while (block.thisBlockHash.substr(0, difficulty) != std::string(difficulty, '0')){
		block.nonce++;
		block.thisBlockHash = block.calculateHash();
	}
	return block;
}

