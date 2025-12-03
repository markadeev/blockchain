#pragma once
#include <unordered_map>

#include "block.h"
#include "transaction.h"

class Node {
public:
	std::vector<Block> chain;
	std::vector<Transaction> mempool;
	std::unordered_map<std::string, std::unordered_map<int, TxOutput>> utxoset;
	
	void receiveTransaction();
	bool verifyTransaction(Transaction &tx);
	void addToMempool(Transaction &tx);
	bool verifyBlock(Block& block);
	void addBlockToChain(Block& block);

};
