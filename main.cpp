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

	miner1.addPeer(&node1);
	node1.addPeer(&miner1);

	Block block = miner1.buildBlock();
	block = miner1.mineBlock(block);
	miner1.broadcastBlock(block);
	
	Wallet wallet1(&node1);

	miner1.minerWallet.buildSubmitTransaction(wallet1.publicKey, 10);
	
	miner1.mineBroadcastBlock();


	node1.blockchain.print();
	


        return 0;
}
