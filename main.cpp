#include <iostream>
#include "block.h"
#include "blockchain.h"
#include "wallet.h"

int main(){

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

	Wallet wallet1;
	wallet1.generateKeys();
	wallet1.getPublicKey();
	std::cout << wallet1.publicKey << std::endl;

        return 0;
}

