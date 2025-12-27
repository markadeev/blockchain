#include <iostream>
#include <unordered_map>

#include "node.h"


bool Node::verifyTransaction(Transaction &tx){
	
	if (tx.inputs.empty()) return false;
	// coinbase transaction
	if (tx.inputs[0].prevTxId == "0" && tx.inputs[0].prevTxIndex == 0) return true;

	if (utxoset.empty()) return false;


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

void Node::addPeer(Node* nodeptr){
	peers.push_back(nodeptr);
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
		updateMempool(block);
		updateUtxos(block);
		broadcastBlock(block);
	}
		
}
bool Node::verifyBlock(Block& block){
	if (blockchain.isEmpty()){
		if (block.prevBlockHash != "0") return false;
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
void Node::updateUtxos(Block& block){
	for (Transaction& tx : block.transactions){
		for (TxInput& txin : tx.inputs){
			utxoset[tx.TxId].erase(txin.prevTxIndex);
		}
		for (TxOutput& txout : tx.outputs){
			utxoset[tx.TxId][txout.index] = txout;
		}
	}
}
void Node::updateMempool(Block& block){
	for (Transaction& tx : block.transactions){
		for (int i = mempool.size(); i-- > 0;){
			if (tx.TxId == mempool[i].TxId){
				mempool.erase(mempool.begin() + i);
			}
		}
	}
}

std::vector<std::pair<std::string, TxOutput>> Node::getUtxos(std::string publicKey){
	std::vector<std::pair<std::string, TxOutput>> utxos;
	for (auto& [txid, voutmap] : utxoset){
		for (auto& [index, txout] : voutmap){
			if (txout.publicKey == publicKey){
				utxos.push_back({txid, txout});
			}
		}
	}
	return utxos;

}
void Node::printUtxoset(){
	std::cout << "Utxoset:" << std::endl;
	if (utxoset.empty()) {
		std::cout << "utxoset is empty" << std::endl;
		return;
	}
	for (auto& [txid, voutmap] : utxoset){
		std::cout << "txid: " << txid << std::endl;
		for (auto& [index, txout] : voutmap){
			std::cout << "txout.index: " << txout.index << std::endl;
			std::cout << "txout.amount: " << txout.amount << std::endl;
			std::cout << "txout.publicKey: " << txout.publicKey << "\n" << std::endl;
		}
	}

}
void Node::printMempool(){
	std::cout << "Mempool:" << std::endl;
	for (Transaction tx : mempool){
		tx.print();
	}
}
