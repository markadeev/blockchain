#include <string>
#include <openssl/sha.h>

class Transaction{
public:
        std::string payer;
        std::string payee;
        int amount;
        std::string signature;

	std::string serializeTransaction();

	bool verifyTransaction(const std::string& publicKeyPEM);

};

