#include "miner.h"

Block Miner::buildBlock(){
	Block block;
	block.prevBlockHash = chain.back().calculateHash();
	block.MerkleRoot = block.calculateMerkleRoot();
	int blockTransactionSize = 5;

	for (int i = 0; i < blockTransactionSize && i < mempool.size(); i++){
		block.transactions.push_back(mempool[i]);
	}

	return block;
}
Block Miner::mineBlock(Block& block){

	int difficulty = 4;
	std::string thisBlockHash = block.calculateHash();

	while (thisBlockHash.substr(0, difficulty) != std::string(difficulty, '0')){
		block.nonce++;
		thisBlockHash = block.calculateHash();
	}
	
	if (verifyBlock(block)) {
		seenBlockSet.insert(block.calculateHash());
		addBlockToChain(block);
	}

	return block;
}

