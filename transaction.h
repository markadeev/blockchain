#include <string>
#include <openssl/sha.h>

struct TxInput{
	std::string prevTxId;
	int index;
	std::string signature;
	std::string publicKey;
};
struct TxOutput{
	int amount;
	std::string publicKeyHash;
};

class Transaction{
public:
	std::string TxId;
        std::vector<TxInput> inputs;
        std::vector<TxOutput> outputs;
        std::string signature;

	std::string serializeTransaction();

	bool verifyTxInput(const std::string& publicKeyPEM);
	bool verifyTransaction(const std::string& publicKeyPEM);

};

