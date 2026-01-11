#include <iostream>
#include <string>
#include <unordered_map>
#include "blockchain.h"
#include "wallet.h"
#include "node.h"
#include "miner.h"
#include "foundation_priv.h"

int main(){

	Miner miner1;	
	Node node1;

	miner1.addPeer(&node1);
	node1.addPeer(&miner1);

	miner1.mineBroadcastBlock();
	
	Wallet wallet1(&node1);
	miner1.minerWallet.buildSubmitTransaction(wallet1.publicKey, 10);
	
	miner1.mineBroadcastBlock();


	Wallet foundationWallet(&node1);

	foundationWallet.importPrivateKeyPEM(FOUNDATION_PRIVKEY);

	auto it = node1.getUtxos(foundationWallet.publicKey);
	
	if (it.empty()){
		std::cout << "myUtxos is empty" << std::endl;
	}
	for (auto& [txid, txout] : it){
		std::cout << "txid: " << txid << std::endl;
		std::cout << "txout.index: " << txout.index << std::endl;
		std::cout << "txout.amount: " << txout.amount << std::endl;
		std::cout << "txout.publicKey: " << txout.publicKey << "\n" << std::endl;
	}



	foundationWallet.buildSubmitTransaction(wallet1.publicKey, 100);
	miner1.mineBroadcastBlock();
	node1.printBlockchain();
	

        return 0;
}
