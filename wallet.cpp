#include <iostream>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/bio.h>
#include <vector>
#include <iomanip>
#include "wallet.h"

Wallet::Wallet(Node* connectedNodeptr)
	:publicKey(""),
	myUtxos({}),
	connectedNode(connectedNodeptr)
{
	generateKeys();
	updateMyUtxos();
}

void Wallet::generateKeys() {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, nullptr);
    if (!ctx) throw std::runtime_error("Failed to create PKEY context");

    if (EVP_PKEY_keygen_init(ctx) <= 0)
        throw std::runtime_error("Keygen init failed");

    if (EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ctx, NID_secp256k1) <= 0)
        throw std::runtime_error("Failed to set curve");

    EVP_PKEY* newkey = nullptr;
    if (EVP_PKEY_keygen(ctx, &newkey) <= 0)
        throw std::runtime_error("Keygen failed");

    pkey = newkey;
    EVP_PKEY_CTX_free(ctx);

    getPublicKey();
}

void Wallet::importPrivateKeyPEM(const std::string& pemkey){
	BIO* bio = BIO_new_mem_buf(pemkey.data(), static_cast<int>(pemkey.size()));
	if (!bio) throw std::runtime_error("BIO creation failed");
	EVP_PKEY* newkey = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
	BIO_free(bio);

	if (!newkey) throw std::runtime_error("Failed to read PEM private key");
	if (pkey) EVP_PKEY_free(pkey);

	pkey = newkey;

	getPublicKey();
	myUtxos = {};
	updateMyUtxos();
}


void Wallet::getPublicKey(){
	BIO* bio = BIO_new(BIO_s_mem());
	PEM_write_bio_PUBKEY(bio, pkey);
	char* data;
	long len = BIO_get_mem_data(bio, &data);
	publicKey.assign(data, len);
	BIO_free(bio);
}

void Wallet::printPublicKey(){
	std::cout << publicKey << std::endl;
}

void Wallet::printPrivateKey(){
	BIO* bio = BIO_new(BIO_s_mem());
	PEM_write_bio_PrivateKey(bio, pkey, nullptr, nullptr, 0, nullptr, nullptr);
	char* data;
	long len = BIO_get_mem_data(bio, &data);
	std::cout.write(data, len);
	std::cout << std::endl;
	BIO_free(bio);
}

EVP_PKEY* Wallet::getPrivateKey(){
	return pkey;
}
Transaction Wallet::buildTransaction(std::string receiverPublicKey, int amount){
	Transaction transaction;
	TxInput txin;
	TxOutput txout;
	TxOutput utxo;

	if (myUtxos.empty()) {
		std::cout << "myUtxos is empty\n" << std::endl;
		return transaction;
	}

	for (auto [txid, txOutput] : myUtxos){
		if (txOutput.amount >= amount){
			utxo = txOutput;
			txin.prevTxId = txid;
			break;
		}
	}
	
	txin.prevTxIndex = utxo.index;
	transaction.inputs.push_back(txin);

	int sendamount = amount;
	int change = utxo.amount - sendamount;
	
	txout.index = 0;
	txout.amount = sendamount;
	txout.publicKey = receiverPublicKey; 

	transaction.outputs.push_back(txout);

	txout.index = 1;
	txout.amount = change;
	txout.publicKey = publicKey;
	
	transaction.outputs.push_back(txout);

	signTransaction(transaction);
	
	transaction.makeTxId();;
	
	return transaction;
}

void Wallet::buildSubmitTransaction(std::string receiverPublicKey, int amount){
	updateMyUtxos();
	Transaction tx = buildTransaction(receiverPublicKey, amount);
	submitTransaction(tx);
}
void Wallet::scanUtxoSet(std::unordered_map<std::string, std::unordered_map<int, TxOutput>>& utxoset){
	myUtxos = {};
	for (auto& [txid, voutmap] : utxoset){
		for (auto& [index, txout] : voutmap){
			if (txout.publicKey == publicKey) myUtxos.push_back({txid, txout});
		}
	}
}

void Wallet::signTransaction(Transaction& tx){
        std::string data = tx.serializeTransaction();

        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestSignInit(ctx, nullptr, EVP_sha256(), nullptr, pkey);
        EVP_DigestSignUpdate(ctx, data.data(), data.size());

        size_t siglen = 0;
        EVP_DigestSignFinal(ctx, nullptr, &siglen);

        std::vector<unsigned char> sig(siglen);
        EVP_DigestSignFinal(ctx, sig.data(), &siglen);

        EVP_MD_CTX_free(ctx);
        sig.resize(siglen);

        std::stringstream ss;
        for (auto c : sig){
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
        }
	for (TxInput& txin : tx.inputs){
		txin.signature = ss.str();
	}
}
void Wallet::submitTransaction(Transaction& tx){
	if (connectedNode == nullptr){
		std::cout << "No connected node" << std::endl;
		return;
	}
	connectedNode->receiveTransaction(tx);
}

void Wallet::updateMyUtxos(){
	if (connectedNode == nullptr){
		std::cout << "No connected node" << std::endl;
		return;
	}
	myUtxos = connectedNode->getMyUtxos(publicKey);


}
void Wallet::printMyUtxos(){
	if (myUtxos.empty()){
		std::cout << "myUtxos is empty" << std::endl;
	}
	for (auto& [txid, txout] : myUtxos){
		std::cout << "txid: " << txid << std::endl;
		std::cout << "txout.index: " << txout.index << std::endl;
		std::cout << "txout.amount: " << txout.amount << std::endl;
		std::cout << "txout.publicKey: " << txout.publicKey << "\n" << std::endl;
	}
}

