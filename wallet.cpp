#include <iostream>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include "wallet.h"


void Wallet::generateKeys(){
	pkey = nullptr;
	pkey = EVP_RSA_gen(2048);
}
void Wallet::getPublicKey(){
	BIO* bio = BIO_new(BIO_s_mem());
	PEM_write_bio_PUBKEY(bio, pkey);
	char* data;
	long len = BIO_get_mem_data(bio, &data);
	publicKey.assign(data, len);
	std::cout << std::endl;
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
