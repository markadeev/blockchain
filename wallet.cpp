#include <iostream>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <vector>
#include <iomanip>
#include "wallet.h"

Wallet::Wallet()
	:publicKey("")
{
	generateKeys();
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


void Wallet::getPublicKey(){
	BIO* bio = BIO_new(BIO_s_mem());
	PEM_write_bio_PUBKEY(bio, pkey);
	char* data;
	long len = BIO_get_mem_data(bio, &data);
	publicKey.assign(data, len);
	BIO_free(bio);
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
void Wallet::scanUtxoSet(std::unordered_map<std::string, std::unordered_map<int, TxOutput>>& utxoset){
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
void Wallet::broadcastTransaction(Transaction& tx){
	for (Node* peer : peers){
		peer->receiveTransaction(tx);
	}
}

