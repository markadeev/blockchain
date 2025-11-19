#include <string>
#include <sstream>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include "transaction.h"

std::string Transaction::serializeTransaction(){
	std::stringstream ss;
	ss << payer << payee << amount;
	return ss.str();
}

bool Transaction::verifyTransaction(const std::string& publicKeyPEM){
	std::vector<unsigned char> sig;
	sig.reserve(signature.size()/2);
	
	for (size_t i = 0; i < signature.size(); i += 2){
		unsigned char b = std::stoi(signature.substr(i, 2), nullptr, 16);
		sig.push_back(b);
	}
	
	BIO* bio = BIO_new_mem_buf(publicKeyPEM.data(), publicKeyPEM.size());
	EVP_PKEY* pubkey = PEM_read_bio_PUBKEY(bio, nullptr, nullptr, nullptr);
	BIO_free(bio);

	std::string data = serializeTransaction();
	EVP_MD_CTX* ctx = EVP_MD_CTX_new();

	EVP_DigestVerifyInit(ctx, nullptr, EVP_sha256(), nullptr, pubkey);
	EVP_DigestVerifyUpdate(ctx, data.data(), data.size());
	int ok = EVP_DigestVerifyFinal(ctx, sig.data(), sig.size());
	
	EVP_MD_CTX_free(ctx);
	EVP_PKEY_free(pubkey);
	
	return ok == 1;
}
