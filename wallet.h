#include <openssl/evp.h>
#include <openssl/pem.h>
#include <string>

class Wallet{
public:
        std::string publicKey;

        void generateKeys();
        void getPublicKey();
        void printPrivateKey();

private:
        EVP_PKEY* pkey;
};
