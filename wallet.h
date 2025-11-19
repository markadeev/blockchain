#include <openssl/evp.h>
#include <openssl/pem.h>
#include <string>
#include "transaction.h"

class Wallet{
public:
        std::string publicKey;

        void generateKeys();
        void getPublicKey();
        void printPrivateKey();
	EVP_PKEY* getPrivateKey();
	void signTransaction(Transaction& tx);


private:
        EVP_PKEY* pkey;
};
