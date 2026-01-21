#include "iostream"
#include "miner.h"
#include "settings.h"

Miner::Miner()
	:minerWallet(this)
{
}
Block Miner::buildBlock(){
	Block block;
	block.prevBlockHash = blockchain.lastBlock().calculateHash();

	for (int i = 0; i < BLOCK_SIZE && i < mempool.size(); i++){
		block.transactions.push_back(mempool[i]);

	}

	Transaction coinbaseTx = buildCoinbaseTransaction();
	block.transactions.push_back(coinbaseTx);

	block.MerkleRoot = block.calculateMerkleRoot();

	return block;
}
Block Miner::mineBlock(Block& block){

	std::string thisBlockHash = block.calculateHash();

	while (thisBlockHash.substr(0, MINING_DIFFICULTY) != std::string(MINING_DIFFICULTY, '0')){
		block.nonce++;
		thisBlockHash = block.calculateHash();
	}
	
	if (verifyBlock(block)) {
		seenBlockSet.insert(block.calculateHash());
		addBlockToChain(block);
		updateMempool(block);
		updateUtxos(block);
		minerWallet.updateMyUtxos();
	}

	return block;
}
void Miner::mineBroadcastBlock(){
	Block block = buildBlock();
	block = mineBlock(block);
	broadcastBlock(block);

}
Transaction Miner::buildCoinbaseTransaction(){
	Transaction transaction;

	TxInput txin;
	txin.prevTxId = "0";
	txin.prevTxIndex = 0;
	// making TxId unique
	// using block height instead of signature
	txin.signature = std::to_string(blockchain.chainSize() + 1);

	TxOutput txout;
	txout.index = 0;
	txout.amount = MINING_REWARD;
	txout.publicKey = minerWallet.publicKey;

	transaction.inputs.push_back(txin);
	transaction.outputs.push_back(txout);

	transaction.makeTxId();;
	
	return transaction;
}

