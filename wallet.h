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

	Wallet();
        void generateKeys();
        void getPublicKey();
        void printPrivateKey();
	std::vector<std::pair<std::string, TxOutput>> myUtxos;
	std::vector<Node*> peers;

	EVP_PKEY* getPrivateKey();
	Transaction buildTransaction(std::string receiverPublicKey, int amount);
	Transaction buildCoinbaseTransaction(std::string reveiverPublicKey, int amount);
	void scanUtxoSet(std::unordered_map<std::string, std::unordered_map<int, TxOutput>>& utxoset);
	void signTransaction(Transaction& tx);
	void broadcastTransaction(Transaction& tx);


private:
        EVP_PKEY* pkey;
};
