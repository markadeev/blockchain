#include <iostream>
#include <unordered_map>

#include "node.h"


bool Node::verifyTransaction(Transaction &tx){
	std::string senderPublicKey = utxoset[tx.inputs[0].prevTxId][tx.inputs[0].prevTxIndex].publicKey;
	std::string data = tx.serializeTransaction();
	for (TxInput& txin : tx.inputs){
		if (!txin.verifyTxInput(senderPublicKey, data)) return false;
	}
	addToMempool(tx);
	return true;
}

void Node::addToMempool(Transaction &tx){
	mempool.push_back(tx);
}


