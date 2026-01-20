#include <iostream>
#include <vector>
#include "blockchain.h"
#include "foundation_pub.h"
#include "settings.h"

Blockchain::Blockchain()
	:chain({})
{
	createGenesisBlock();
}

void Blockchain::createGenesisBlock(){

	Transaction tx;
	TxOutput txout;
	txout.index = 0;
	txout.amount = FOUNDATION_AMOUNT;
	txout.publicKey = FOUNDATION_PUBKEY;
	tx.outputs.push_back(txout);
	tx.makeTxId();

	Block genesisBlock;
	genesisBlock.transactions.push_back(tx);
	chain.push_back(genesisBlock);
}
Block& Blockchain::lastBlock(){
	return chain.back();
}
void Blockchain::addBlock(Block& block){
	chain.push_back(block);
}
bool Blockchain::isEmpty(){
	return chain.empty();	
}
void Blockchain::print(){
	std::cout << "Blockchain:" << std::endl;
	for (int i = 0; i < chain.size() ; i++){
		std::cout << "Block " << i << std::endl;
		chain[i].print();
	}
}

