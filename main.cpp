#include <iostream>
#include <string>
#include <unordered_map>
#include "blockchain.h"
#include "wallet.h"
#include "node.h"
#include "miner.h"

int main(){

	Miner miner1;	
	Node node1;
	miner1.peers.push_back(&node1);
	node1.peers.push_back(&miner1);

	Block block = miner1.buildBlock();
	block = miner1.mineBlock(block);
	miner1.broadcastBlock(block);
	
	Wallet wallet1;
	Transaction tx1 = miner1.minerWallet.buildTransaction(wallet1.publicKey, 10);
	
	miner1.minerWallet.connectedNode = &node1;
	miner1.minerWallet.submitTransaction(tx1);

	miner1.printMempool();
	node1.printMempool();

	Block block2 = miner1.buildBlock();
	block2 = miner1.mineBlock(block2);
	miner1.broadcastBlock(block2);

	miner1.printMempool();
	node1.printMempool();

	


        return 0;
}
