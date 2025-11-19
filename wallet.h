#include <openssl/evp.h>
#include <openssl/pem.h>
#include <string>

class Wallet{
public:
        std::string publicKey;

        void generateKeys();
        void getPublicKey();
        void printPrivateKey();
	EVP_PKEY* getPrivateKey();

private:
        EVP_PKEY* pkey;
};
