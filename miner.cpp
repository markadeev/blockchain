#include "miner.h"
#include "iostream"

Block Miner::buildBlock(){
	Block block;
	block.prevBlockHash = blockchain.lastBlock().calculateHash();

	int blockTransactionSize = 5;
	for (int i = 0; i < blockTransactionSize && i < mempool.size(); i++){
		block.transactions.push_back(mempool[i]);
		mempool.erase(mempool.begin() + i);

	}

	Transaction coinbaseTx = buildCoinbaseTransaction(minerWallet.publicKey, 50);
	block.transactions.push_back(coinbaseTx);

	block.MerkleRoot = block.calculateMerkleRoot();

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
		updateUtxos(block);
		minerWallet.scanUtxoSet(utxoset);
	}

	return block;
}
Transaction Miner::buildCoinbaseTransaction(std::string minerWalletPublicKey, int amount){
	Transaction transaction;

	TxInput txin;
	txin.prevTxId = "0";
	txin.prevTxIndex = 0;
	// making TxId unique
	// using block height instead of signature
	txin.signature = std::to_string(blockchain.chain.size() + 1);

	TxOutput txout;
	txout.index = 0;
	txout.amount = amount;
	txout.publicKey = minerWalletPublicKey;

	transaction.inputs.push_back(txin);
	transaction.outputs.push_back(txout);

	transaction.makeTxId();;
	
	return transaction;
}

