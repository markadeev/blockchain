#include <iostream>
#include <unordered_map>

#include "node.h"


bool Node::verifyTransaction(Transaction &tx){
	if (utxoset.empty()) return false;
	if (tx.inputs.empty()) return false;

	// must be wrapped correctly

	std::string data = tx.serializeTransaction();

	for (TxInput& txin : tx.inputs){
		auto itOuter = utxoset.find(txin.prevTxId);
		if (itOuter == utxoset.end()) return false;
		auto& innerMap = itOuter->second;
		auto itInner = innerMap.find(txin.prevTxIndex);
		if (itInner == innerMap.end()) return false;

		TxOutput& prevTxOut = itInner->second;
		
		std::string senderPublicKey = prevTxOut.publicKey;
		if (!txin.verifyTxInput(senderPublicKey, data)) return false;
	}
	return true;
}

void Node::addToMempool(Transaction &tx){
	mempool.push_back(tx);
}

void Node::broadcastTransaction(Transaction& tx){
	for (Node* peer : peers){
		peer->receiveTransaction(tx);
	}
}
void Node::receiveTransaction(Transaction& tx){
	if (seenTxSet.count(tx.TxId)) return;
	if (verifyTransaction(tx)){
		seenTxSet.insert(tx.TxId);
		addToMempool(tx);
		broadcastTransaction(tx);
	}
}

void Node::broadcastBlock(Block& block){
	for (Node* peer : peers){
		peer->receiveBlock(block);
	}
}
void Node::receiveBlock(Block& block){
	if (seenBlockSet.count(block.calculateHash())) return;
	if (verifyBlock(block)){
		seenBlockSet.insert(block.calculateHash());
		addBlockToChain(block);
		broadcastBlock(block);
	}
		
}
bool Node::verifyBlock(Block& block){
	if (blockchain.isEmpty()){
		if (block.prevBlockHash != "") return false;
	} else {
		if (block.prevBlockHash != blockchain.lastBlock().calculateHash()) return false;
		if (block.timestamp < blockchain.lastBlock().timestamp) return false;
	}
	
	std::string thisBlockHash = block.calculateHash();
	if (thisBlockHash.substr(0, 4) != std::string(4, '0')) return false;


	for (Transaction& tx : block.transactions){
		if (!verifyTransaction(tx)) return false;
	}

	return true;
}
void Node::addBlockToChain(Block& block){
	blockchain.addBlock(block);
}


