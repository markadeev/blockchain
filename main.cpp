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
	//miner1.addToMempool(TestTransaction);

	Block prevBlock;
	miner1.chain.push_back(prevBlock);

	Node node2;
	node2.utxoset = utxoset;
	Node node3;
	node3.utxoset = utxoset;
	miner1.utxoset = utxoset;

	node1.peers.push_back(&node2);
	node2.peers.push_back(&node3);
	node3.peers.push_back(&miner1);

	//miner1.peers.push_back(&node3);

	node1.broadcastTransaction(TestTransaction);

	std::cout << "node 2 mempool" << std::endl;
	for (auto tx : node2.mempool){
		tx.print();
	}
	
	std::cout << "miner 1 mempool" << std::endl;
	for (auto tx : miner1.mempool){
		tx.print();
	}
	Block block = miner1.buildBlock();
	block = miner1.mineBlock(block);

	std::cout << "miner 1 mined block: " << std::endl;
	block.print();
	std::cout << "miner1 block verification:" << miner1.verifyBlock(block) << std::endl;
	miner1.broadcastBlock(block);
	std::cout << "miner1 last block:" << std::endl;
	miner1.chain.back().print();

        return 0;
}
