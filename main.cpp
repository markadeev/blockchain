#include <iostream>
#include "block.h"
#include "blockchain.h"
#include "wallet.h"

int main(){
/*
	Blockchain testcoin;
	
	testcoin.createGenesisBlock();
	testcoin.getLatestBlock().mineBlock();
	std::cout << testcoin.getLatestBlock().nonce << std::endl;
	std::cout << testcoin.getLatestBlock().thisBlockHash << std::endl;
	std::cout << std::ctime(&testcoin.getLatestBlock().timestamp) << std::endl;
	
	Block block2(testcoin.getLatestBlock().thisBlockHash);
	block2.mineBlock();
	testcoin.addBlock(block2);

	std::cout << testcoin.getLatestBlock().nonce << std::endl;
	std::cout << testcoin.getLatestBlock().thisBlockHash << std::endl;
	std::cout << std::ctime(&testcoin.getLatestBlock().timestamp) << std::endl;
*/

	Wallet wallet1;
	wallet1.generateKeys();
	std::cout << "Wallet 1 public Key: \n" << wallet1.publicKey << std::endl;

	Transaction tx1;
	wallet1.signTransaction(tx1);
	std::cout << "Transaction 1 valid: " <<  tx1.verifyTransaction(wallet1.publicKey) << std::endl;

	Wallet wallet2;
	wallet2.generateKeys();

	Transaction tx2;
	wallet2.signTransaction(tx2);
	std::cout << "Transaction 2 valid: " << tx2.verifyTransaction(wallet2.publicKey) << std::endl;

        return 0;
}
