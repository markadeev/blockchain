#include <iostream>
#include <string>
#include <unordered_map>
#include "blockchain.h"
#include "wallet.h"
#include "node.h"
#include "miner.h"

int main(){
	Blockchain bc;
	bc.lastBlock().print();

	Node node1;
	node1.blockchain.lastBlock().print();

	Node node2;
	node1.peers.push_back(&node2);




        return 0;
}
