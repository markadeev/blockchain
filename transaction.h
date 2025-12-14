#pragma once
#include <string>
#include <vector>
#include <openssl/sha.h>

struct TxInput{
	std::string prevTxId;
	int prevTxIndex;
	// use P2PK, add P2PKH later
	// in P2PK, the scriptsig unlocking script has only the publicKey
	std::string signature;
	bool verifyTxInput(const std::string& publicKeyPEM, std::string data);
};
struct TxOutput{
	int index;
	int amount;
	std::string publicKey;
};

class Transaction{
public:
	std::string TxId;
        std::vector<TxInput> inputs;
        std::vector<TxOutput> outputs;

	std::string serializeTransaction();

	bool verifyTransaction(const std::string& publicKeyPEM);
	void makeTxId();
	std::string calculateHash();

};

