#include <iostream>
#include <string>
#include <unordered_map>
#include "blockchain.h"
#include "wallet.h"
#include "node.h"
#include "miner.h"

int main(){
	std::unordered_map<std::string, std::unordered_map<int, TxOutput>> utxoset;

	Wallet wallet1;
	wallet1.generateKeys();

	Wallet wallet2;
	wallet2.generateKeys();

	TxInput tin;
	tin.prevTxId = "s0as0df0asd0f";
	tin.prevTxIndex = 0;

	TxOutput tout;
	tout.index = 0;
	tout.amount = 50;
	tout.publicKey = wallet1.publicKey;

	Transaction tx;
	tx.inputs.push_back(tin);
	tx.outputs.push_back(tout);

	wallet1.signTransaction(tx);

	tx.makeTxId();

	utxoset[tx.TxId][0] = tx.outputs[0];
	wallet1.scanUtxoSet(utxoset);



	Transaction TestTransaction = wallet1.buildTransaction(wallet2.publicKey, 30);

	Node node1;
	node1.utxoset = utxoset;

	Miner miner1;
	miner1.addToMempool(TestTransaction);
	Block prevBlock;

	miner1.chain.push_back(prevBlock);

	Block block = miner1.buildBlock();
	block = miner1.mineBlock(block);
	std::cout << "miner 1 mined block: " << block.serializeBlock() << std::endl;
	std::cout << "block merkle root:" << block.calculateMerkleRoot() << std::endl;

	Node node2;
	std::cout << "node2 utxoset empty? " << node2.utxoset.empty() << std::endl;
	node2.utxoset = utxoset;
	std::cout << "node2 utxoset empty? " << node2.utxoset.empty() << std::endl;
	Node node3;
	node3.utxoset = utxoset;
	miner1.utxoset = utxoset;
	node1.peers.push_back(&node2);
	node2.peers.push_back(&node3);
	node3.peers.push_back(&miner1);

	node1.broadcastTransaction(TestTransaction);
	if (!miner1.mempool.empty()) std::cout << miner1.mempool[0].serializeTransaction() << std::endl;

	

        return 0;
}
