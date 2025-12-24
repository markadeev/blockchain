#include <iostream>
#include <string>
#include <unordered_map>
#include "blockchain.h"
#include "wallet.h"
#include "node.h"
#include "miner.h"

int main(){
	Wallet wallet1;
	Wallet wallet2;

	//Transaction tx1 = wallet1.buildTransaction(wallet2.publicKey, 10);
	//tx1.print();

	Miner miner1;	
	Block block = miner1.buildBlock();
	block = miner1.mineBlock(block);
	//miner1.blockchain.print();

	Transaction tx2 = miner1.minerWallet.buildTransaction(wallet1.publicKey, 10);
	tx2.print();

	



        return 0;
}
