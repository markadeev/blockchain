#include <iostream>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <vector>
#include <iomanip>
#include "wallet.h"


void Wallet::generateKeys(){
	pkey = nullptr;
	pkey = EVP_RSA_gen(2048);
	
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

