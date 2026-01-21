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

	Wallet foundationWallet(&node1);
	foundationWallet.importPrivateKeyPEM(FOUNDATION_PRIVKEY);

	Wallet wallet1(&node1);
	foundationWallet.buildSubmitTransaction(wallet1.publicKey, 900);

	miner1.mineBroadcastBlock();
	node1.printBlockchain();

        return 0;
}
