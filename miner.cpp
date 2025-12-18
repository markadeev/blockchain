#include "miner.h"

Block Miner::buildBlock(){
	Block block;
	block.prevBlockHash = blockchain.lastBlock().calculateHash();
	block.MerkleRoot = block.calculateMerkleRoot();
	int blockTransactionSize = 5;

	for (int i = 0; i < blockTransactionSize && i < mempool.size(); i++){
		block.transactions.push_back(mempool[i]);
	}
	Transaction coinbaseTx = buildCoinbaseTransaction(minerWallet.publicKey, 50);
	block.transactions.push_back(coinbaseTx);

	return block;
}
Block Miner::mineBlock(Block& block){

	int difficulty = 4;
	std::string thisBlockHash = block.calculateHash();

	while (thisBlockHash.substr(0, difficulty) != std::string(difficulty, '0')){
		block.nonce++;
		thisBlockHash = block.calculateHash();
	}
	
	if (verifyBlock(block)) {
		seenBlockSet.insert(block.calculateHash());
		addBlockToChain(block);
		removeUtxos(block);
	}

	return block;
}
Transaction Miner::buildCoinbaseTransaction(std::string minerWalletPublicKey, int amount){
	Transaction transaction;

	TxInput txin;
	txin.prevTxId = "0";
	txin.prevTxIndex = 0;

	TxOutput txout;
	txout.index = 0;
	txout.amount = amount;
	txout.publicKey = minerWalletPublicKey;

	transaction.inputs.push_back(txin);
	transaction.outputs.push_back(txout);

	transaction.makeTxId();;
	
	return transaction;
}

