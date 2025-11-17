#include <iostream>
#include "block.h"

int main(){
        Block block1("000000");
	block1.mineBlock();
        Block block2(block1.calculateHash());
	block2.mineBlock();
        Block block3(block2.calculateHash());
	block3.mineBlock();

        block1.calculateHash();
	block1.nonce = 0;

	std::cout << "Block1.nonce: " << block1.nonce << std::endl;
	std::cout << "Block1.thisBlockHash: " << block1.thisBlockHash << std::endl;
	std::cout << "Block1.timestamp: " << std::ctime(&block1.timestamp) << std::endl;

	std::cout << "Block2.nonce: " << block2.nonce << std::endl;
	std::cout << "Block2.thisBlockHash: " << block2.thisBlockHash << std::endl;
	std::cout << "Block2.timestamp: " << std::ctime(&block2.timestamp) << std::endl;

	std::cout << "Block3.nonce: " << block3.nonce << std::endl;
	std::cout << "Block3.thisBlockHash: " << block3.thisBlockHash << std::endl;
	std::cout << "Block3.timestamp: " << std::ctime(&block3.timestamp) << std::endl;

        return 0;
}

