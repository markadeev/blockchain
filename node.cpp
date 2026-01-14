#include <iostream>
#include <unordered_map>

#include "node.h"

Node::Node()
{
	updateUtxos(blockchain.lastBlock());
}


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
		if (!txin.verifyTxInputSignature(senderPublicKey, data)) return false;
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
//helper function for verifyBlock
bool Node::containsUtxo(
		const std::unordered_map<std::string, std::unordered_map<int, TxOutput>>& utxoset, 
		const std::string& txid, 
		int index) {

	// skip coinbase transaction
	if (txid == "0" && index == 0) return true;

	// search in outer map
	auto txIt = utxoset.find(txid);
	if (txIt == utxoset.end()){
		std::cout << "Node.containsUtxo(): txid not found in utxoset" << std::endl;
		return false;
	}

	// search in inner map
	return txIt->second.find(index) != txIt->second.end();
}
//helper function for verifyBlock
void Node::eraseUtxo(
		std::unordered_map<std::string, std::unordered_map<int, TxOutput>>& utxoset,
    		const std::string& txid,
    		int index) {
	auto txIt = utxoset.find(txid);
	if (txIt == utxoset.end()) return;

	txIt->second.erase(index);

	// clean up empty inner map
	if (txIt->second.empty())
	utxoset.erase(txIt);
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

	// check for double spending
	// create temporary copy of utxoset, add or remove utxos dynamically
	auto tempUtxos = utxoset;
	for (Transaction& tx : block.transactions){
		for (TxInput& txin : tx.inputs){
			// if no such utxo => invalid block
			if (!containsUtxo(tempUtxos, txin.prevTxId, txin.prevTxIndex)){
				std::cout << "Node.verifyBlock(): Invalid block" << std::endl;
				return false;
			}
			// if there is such utxo, erase it from temporary utxoset
			eraseUtxo(tempUtxos, txin.prevTxId, txin.prevTxIndex);
		}
		// in case tx1 outputs are used by tx2 in the same block
		// add txouts dynamically
		for (TxOutput& txout : tx.outputs){
			tempUtxos[tx.TxId][txout.index] = txout;
		}
	}

	return true;
}


void Node::addBlockToChain(Block& block){
	blockchain.addBlock(block);
}
void Node::updateUtxos(Block& block){
	for (Transaction& tx : block.transactions){
		for (TxInput& txin : tx.inputs){
			utxoset[txin.prevTxId].erase(txin.prevTxIndex);
			if (utxoset[txin.prevTxId].empty()) utxoset.erase(txin.prevTxId);
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

std::vector<std::pair<std::string, TxOutput>> Node::getMyUtxos(std::string publicKey){
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
void Node::printBlockchain(){
	blockchain.print();
}
