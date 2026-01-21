#pragma once
#include <string>
#include <vector>
#include <openssl/sha.h>

struct TxInput{
	// previous transaction ID
	std::string prevTxId;
	// index of a transaction output within the previous transaction
	int prevTxIndex;
	// wallet signature of the serialized transaction's hash
	std::string signature;

	// constructor with default prevTxId "" and prevTxIndex 0
	TxInput();
	// convert serialized transaction to raw bytes
	// convert public Key in PEM format to EVP_PKEY* object
	// check if serialized transaction hash is equal to the product of public key and signature
	bool verifyTxInputSignature(const std::string& publicKeyPEM, std::string data);
};
struct TxOutput{
	// index of a transaction output
	int index;
	// transferred amount
	int amount;
	// public Key of the receiver wallet
	std::string publicKey;
	// constructor with default index 0 and amount 0
	TxOutput();
};

class Transaction{
public:
	// transaction ID
	std::string TxId;
	// vector of transaction inputs
        std::vector<TxInput> inputs;
	// vector of transaction outputs
        std::vector<TxOutput> outputs;

	// serialize transaction data: transaction inputs without signatures
	// and transaction outputs
	std::string serializeTransaction();
	// print all transaction data
	void print();

	// verify each transaction input signature
	bool verifyTxSignature(const std::string& publicKeyPEM);
	// serialize all transaction data including signatures
	// double hash, convert to hex string, this is a transaction ID
	void makeTxId();
	// used later to calculate merkle root
	// serialize transaction, hash, convert to hex string
	std::string calculateHash();

};

