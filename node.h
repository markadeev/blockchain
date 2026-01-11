#pragma once
#include <unordered_map>
#include <unordered_set>
#include "blockchain.h"
#include "block.h"
#include "transaction.h"

class Node {
public:

	Blockchain blockchain;
	std::vector<Transaction> mempool;
	std::unordered_map<std::string, std::unordered_map<int, TxOutput>> utxoset;
	std::vector<Node*> peers;	
	std::unordered_set<std::string> seenTxSet;
	std::unordered_set<std::string> seenBlockSet;

	Node();

	bool verifyTransaction(Transaction &tx);
	void addToMempool(Transaction &tx);
	void addPeer(Node* nodeptr);
	
	void broadcastTransaction(Transaction& tx);
	void receiveTransaction(Transaction& tx);

	void broadcastBlock(Block& block);
	void receiveBlock(Block& block);

	bool verifyBlock(Block& block);
	void addBlockToChain(Block& block);

	void updateUtxos(Block& block);
	void updateMempool(Block& block);
	std::vector<std::pair<std::string, TxOutput>> getMyUtxos(std::string publicKey);
	void printUtxoset();
	void printMempool();
	void printBlockchain();
	

};
