#include <iostream>
#include "block.h"
#include "blockchain.h"
#include "wallet.h"

int main(){

	Wallet wallet;
	wallet.generateKeys();

	TxInput tin;
	tin.prevTxId = "s0as0df0asd0f";
	tin.index = 0;
	tin.publicKey = wallet.publicKey;

	TxOutput tout;
	tout.amount = 50;

	Transaction tx;
	tx.inputs.push_back(tin);
	tx.outputs.push_back(tout);

	wallet.signTransaction(tx);
	for (auto tin : tx.inputs){
		std::cout << "Signature: " << tin.signature << std::endl;
	}

	tx.makeTxId();
	std::cout << "TxId: " << tx.TxId << std::endl;

	tx.verifyTransaction(wallet.publicKey);
	std::cout << "Transaction valid: " << tx.verifyTransaction(wallet.publicKey) << std::endl;

        return 0;
}
