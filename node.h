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
	// outer map key is a transaction id, inner map key is a transaction output index
	std::unordered_map<std::string, std::unordered_map<int, TxOutput>> utxoset;
	// peers to submit and receive transactions and blocks
	std::vector<Node*> peers;	
	// seen transaction set to prevent rebroadcasting of seen transactions
	// key is transaction id
	std::unordered_set<std::string> seenTxSet;
	// seen block set to prevent rebroadcasting of seen blocks
	// key is block's hash
	std::unordered_set<std::string> seenBlockSet;

	Node();

	// check if coinbase, whether in utxoset, check signature
	bool verifyTransaction(Transaction &tx);
	void addToMempool(Transaction &tx);
	// add peer to peers vector
	void addPeer(Node* nodeptr);
	
	// broadcast transaction to the "connected" peers
	void broadcastTransaction(Transaction& tx);
	// receive transaction from a peer 
	void receiveTransaction(Transaction& tx);

	// broadcast block to the "connected" peers
	void broadcastBlock(Block& block);
	// receive block from a peer
	void receiveBlock(Block& block);

	// check if genesis block, check timestamp, hashes
	// verify transactions, check double spending
	bool verifyBlock(Block& block);

	// helper function for verifyBlock() to check double spending
	bool containsUtxo(const std::unordered_map<std::string, std::unordered_map<int, TxOutput>>& utxoset, const std::string& txid, int index);
	// helper function for verifyBlock() to check double spending
	void eraseUtxo(std::unordered_map<std::string, std::unordered_map<int, TxOutput>>& utxoset, const std::string& txid, int index);

	void addBlockToChain(Block& block);

	// loop over transactions in the new block, delete used utxos, add new ones
	void updateUtxos(Block& block);
	// loop over transactions in the new block, delete confirmed transactions 
	void updateMempool(Block& block);

	// every wallet has a connectedNode, updates wallet utxos through it
	// wallet.updateMyUtxos() prompts connectedNode->getMyUtxos(wallet.publicKey)
	// node returns a list of utxos with the corresponding publicKey to the wallet
	std::vector<std::pair<std::string, TxOutput>> getMyUtxos(std::string publicKey);
	void printUtxoset();
	void printMempool();
	void printBlockchain();
	

};
