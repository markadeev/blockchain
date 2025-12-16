#include <string>
#include <sstream>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
//#include <vector>
#include <iostream>
#include <iomanip>
#include "transaction.h"

std::string Transaction::serializeTransaction(){
	std::stringstream ss;
	for (TxInput txin : inputs){
		ss << "| txin.prevTxId: " << txin.prevTxId << " |\n| txin.prevTxIndex: " << txin.prevTxIndex << " |\n\n" ;
	}
	for (TxOutput txout : outputs){
		ss << "| txout.amount: " << txout.amount << " |\n| txout.publicKey:\n" << txout.publicKey << "\n";
	}
	return ss.str();
}
void Transaction::print(){
	std::stringstream ss;
	ss << "| TxId: " << TxId << " |\n\n";
	for (TxInput txin : inputs){
		ss << "| txin.prevTxId: " << txin.prevTxId << " |\n| txin.prevTxIndex: " << txin.prevTxIndex << " |\n\n" ;
	}
	for (TxOutput txout : outputs){
		ss << "| txout.amount: " << txout.amount << " |\n| txout.publicKey:\n" << txout.publicKey << "\n";
	}
	std::cout << ss.str() << std::endl;
}

bool TxInput::verifyTxInput(const std::string& publicKeyPEM, std::string data){
	std::vector<unsigned char> sig;
	sig.reserve(signature.size()/2);
	
	for (size_t i = 0; i < signature.size(); i += 2){
		unsigned int byte;
		std::stringstream ss;
		ss << std::hex << signature.substr(i, 2);
		ss >> byte;
		sig.push_back(static_cast<unsigned char>(byte));

	}
	
	BIO* bio = BIO_new_mem_buf(publicKeyPEM.data(), publicKeyPEM.size());
	EVP_PKEY* pubkey = PEM_read_bio_PUBKEY(bio, nullptr, nullptr, nullptr);
	BIO_free(bio);

	EVP_MD_CTX* ctx = EVP_MD_CTX_new();

	EVP_DigestVerifyInit(ctx, nullptr, EVP_sha256(), nullptr, pubkey);
	EVP_DigestVerifyUpdate(ctx, data.data(), data.size());
	int ok = EVP_DigestVerifyFinal(ctx, sig.data(), sig.size());
	
	EVP_MD_CTX_free(ctx);
	EVP_PKEY_free(pubkey);
	
	return ok == 1;
}
bool Transaction::verifyTransaction(const std::string& publicKeyPEM){
	std::string data = serializeTransaction();
	for (TxInput& txin : inputs){
		if (txin.verifyTxInput(publicKeyPEM, data) != 1) return false;
	}
	return true;
}
void Transaction::makeTxId(){
	std::string data = serializeTransaction();

	unsigned char hash1[32];
        SHA256(reinterpret_cast<const unsigned char *>(data.c_str()), data.size(), hash1);

	unsigned char hash2[32];
        SHA256(hash1, 32, hash2);

        std::stringstream os;
        for (int i = 0; i < 32; i++){
                os << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash2[i]);
        }
	TxId = os.str();
}
std::string Transaction::calculateHash(){
	std::string data = serializeTransaction();

	unsigned char hash[32];
        SHA256(reinterpret_cast<const unsigned char *>(data.c_str()), data.size(), hash);

        std::stringstream os;
        for (int i = 0; i < 32; i++){
                os << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }

	return os.str();
}
