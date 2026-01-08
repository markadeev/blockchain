#pragma once
#include "node.h"
#include "block.h"
#include "wallet.h"
#include "transaction.h"

class Miner: public Node {
public:

	Miner();
	Wallet minerWallet;
	Block buildBlock();
	Block mineBlock(Block& block);
	void mineBroadcastBlock();
	Transaction buildCoinbaseTransaction(std::string minerWalletPublicKey, int reward);



};
