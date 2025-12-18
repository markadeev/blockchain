#pragma once
#include "node.h"
#include "block.h"
#include "wallet.h"
#include "transaction.h"

class Miner: public Node {
public:

	Wallet minerWallet;
	Block buildBlock();
	Block mineBlock(Block& block);
	Transaction buildCoinbaseTransaction(std::string minerWalletPublicKey, int amount);



};
