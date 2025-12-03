#include <iostream>
#include <string>
#include <unordered_map>
#include "blockchain.h"
#include "wallet.h"
#include "node.h"

int main(){
	std::unordered_map<std::string, std::unordered_map<int, TxOutput>> utxoset;

	Wallet wallet1;
	wallet1.generateKeys();

	Wallet wallet2;
	wallet2.generateKeys();

	TxInput tin;
	tin.prevTxId = "s0as0df0asd0f";
	tin.prevTxIndex = 0;

	TxOutput tout;
	tout.index = 0;
	tout.amount = 50;
	tout.publicKey = wallet1.publicKey;

	Transaction tx;
	tx.inputs.push_back(tin);
	tx.outputs.push_back(tout);

	wallet1.signTransaction(tx);
	for (auto tin : tx.inputs){
		std::cout << "Signature: " << tin.signature << std::endl;
	}

	tx.makeTxId();

	std::cout << "TxId: " << tx.TxId << std::endl;
	
	utxoset[tx.TxId][0] = tx.outputs[0];
	std::cout << "utxo amount: " << utxoset[tx.TxId][0].amount << " utxo PublicKey: " << utxoset[tx.TxId][0].publicKey << std::endl;

	wallet1.scanUtxoSet(utxoset);

	//for (auto& [txid, txOutput] : wallet1.myUtxos){
//		std::cout << "wallet1 utxos: " << txOutput.amount << txOutput.publicKey << std::endl;
//	}

	//std::cout << "Wallet1 pubkey address: " << wallet1.getPrivateKey() << std::endl;
	//std::cout << "Wallet2 pubkey address: " << wallet2.getPrivateKey() << std::endl;
	//std::cout << "\n\n\nWallet1 publicKey: " << wallet1.publicKey << std::endl;
	//std::cout << "\n\n\nWallet2 publicKey: " << wallet2.publicKey << std::endl;

	Transaction TestTransaction = wallet1.buildTransaction(wallet2.publicKey, 30);
	//std::cout << "Sending mone" << std::endl;
	//std::cout << "TxId: " << TestTransaction.TxId << std::endl;

	//std::cout << "Test Transaction: " << TestTransaction.serializeTransaction() << std::endl;

	//TestTransaction.verifyTransaction(wallet1.publicKey);
	//std::cout << "TestTransaction valid: " << TestTransaction.verifyTransaction(wallet1.publicKey) << std::endl;

	Node node1;
	node1.utxoset = utxoset;
	std::cout << "node1 verify TestTransaction: " << node1.verifyTransaction(TestTransaction) << std::endl;
	for (Transaction tx : node1.mempool){
		std::cout << "Mempool Transaction ID: " << tx.TxId << std::endl;
	}

        return 0;
}
