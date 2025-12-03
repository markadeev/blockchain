#include <iostream>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <vector>
#include <iomanip>
#include "wallet.h"


void Wallet::generateKeys(){
	EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
	EVP_PKEY_keygen_init(ctx);
	EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048);
	EVP_PKEY* newkey = nullptr;
	EVP_PKEY_keygen(ctx, &newkey);

	pkey = newkey;

	EVP_PKEY_CTX_free(ctx);

	
	// extract public key, assign to publicKey field
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

	//sign transaction;
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

