#pragma once
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <string>
#include <unordered_map>
#include "transaction.h"
#include "node.h"


class Wallet{
public:
        std::string publicKey;
	std::vector<std::pair<std::string, TxOutput>> myUtxos;
	Node* connectedNode;

	Wallet();

        void generateKeys();
        void getPublicKey();
        void printPrivateKey();
	EVP_PKEY* getPrivateKey();

	Transaction buildTransaction(std::string receiverPublicKey, int amount);
	void scanUtxoSet(std::unordered_map<std::string, std::unordered_map<int, TxOutput>>& utxoset);
	void signTransaction(Transaction& tx);

	void submitTransaction(Transaction& tx);

	void receiveUtxos();
	void printMyUtxos();


private:
        EVP_PKEY* pkey;
};
