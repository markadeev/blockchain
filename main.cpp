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
	foundationWallet.printPublicKey();

	foundationWallet.importPrivateKeyPEM(FOUNDATION_PRIVKEY);
	foundationWallet.printPublicKey();

	foundationWallet.buildSubmitTransaction(wallet1.publicKey, 100);
	miner1.mineBroadcastBlock();
	

        return 0;
}
