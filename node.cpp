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
	addToMempool(tx);
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
	//if (true){
		seenTxSet.insert(tx.TxId);
		mempool.push_back(tx);
		broadcastTransaction(tx);
	}
}

bool Node::verifyBlock(Block& block){
	// change
	return true;
}
void Node::addBlockToChain(Block& block){
	chain.push_back(block);
}


