#pragma once
#include "node.h"
#include "block.h"
#include "wallet.h"
#include "transaction.h"

class Miner: public Node {
public:
	// miner's own wallet to which mining rewards are transferred
	Wallet minerWallet;

	// construct Miner object with minerWallet 
	// wallet's connected node is Miner itself
	Miner();

	// create block, connect it to the last block on chain,
	// add BLOCK_SIZE amount of transactions from the mempool
	// add coinbase transaction with MINING_REWARD for minerWallet
	// calculate MerkleRoot
	Block buildBlock();
	// increment nonce and hash block
	// until it has the desired MINING_DIFFICULTY amount of leading zeros
	Block mineBlock(Block& block);
	// build, mine and broadcast the block to it's connected peers
	void mineBroadcastBlock();
	// helper function for buildBlock()
	// create a coinbase transaction with MINING_REWARD
	// to the minerWallet with unique Transaction ID
	Transaction buildCoinbaseTransaction();



};
