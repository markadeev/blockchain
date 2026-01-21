#pragma once
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <string>
#include <unordered_map>
#include "transaction.h"
#include "node.h"


class Wallet{
public:
	// this project uses Pay to Public Key model
	// transactions are paid to a certain wallet's publicKey 
	// no hashing, no scriptSig
        std::string publicKey;
	// local list of utxos available to this wallet
	std::vector<std::pair<std::string, TxOutput>> myUtxos;
	// list of pending utxos, used by the wallet but not confirmed yet
	// prevents double spending of it's own utxos before block confirmation
	std::vector<std::pair<std::string, int>> pendingUtxos;
	// connectedNode is used to update wallet's utxos 
	// and to submit transactions to
	Node* connectedNode;

	// constructor builds a wallet with a given connected node
	// generates keys to sign transactions
	Wallet(Node* connectedNodeptr);

	// generate ECDSA key, extract publicKey in PEM format
	// assign it to publicKey field
        void generateKeys();

	// imprort private key
	// calculate public key from it
	// empty and update wallet's utxos for the new key
	// empty pending utxos
	void importPrivateKeyPEM(const std::string& pemkey);

	// extract public key from EVP_PKEY* object in PEM format
	// assign to publicKey field
        void getPublicKey();
	// get private key, for debugging purposes 
	EVP_PKEY* getPrivateKey();

	// print public key in PEM format
	void printPublicKey();
	// strictly for illustration and debugging purposes
	// print private key in PEM format
        void printPrivateKey();

	// helper function for buildTransaction()
	// check if the given transaction is in pendingUtxos
	bool isPending(std::string& txid, TxOutput& txout);
	// check if a utxo with needed amount is available
	// check if this utxo isn't pending
	// calculate change and return it to itself
	// sign and make transaction ID
	Transaction buildTransaction(std::string receiverPublicKey, int amount);

	// serialize transaction, hash, sign with wallet privateKey
	// convert raw signature to hex string
	// assign this signature to all transaction inputs
	void signTransaction(Transaction& tx);
	// prompt connectedNode->receiveTransaction(transaction)
	// which verifies and broadcasts the transaction to all connected nodes
	void submitTransaction(Transaction& tx);
	// update wallet utxos, update pending utxos,
	// build transaction, submit transaction to the connected node
	void buildSubmitTransaction(std::string receiverPublicKey, int amount);

	// prompt connectedNode->getMyUtxos(wallet.publicKey)
	// node returns a vector with wallet's utxos
	// assign it to myUtxos
	void updateMyUtxos();
	// if a transaction is not in wallet utxos anymore 
	// it has been confirmed by the blockchain
	// threfore gets deleted
	void updatePendingUtxos();
	void printMyUtxos();



private:
        EVP_PKEY* pkey;
};
