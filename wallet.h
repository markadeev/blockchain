#include <openssl/evp.h>
#include <openssl/pem.h>
#include <string>
#include <unordered_map>
#include "transaction.h"


class Wallet{
public:
        std::string publicKey;
	std::string publicKeyHash;

        void generateKeys();
        void getPublicKey();
        void printPrivateKey();
	std::vector<std::pair<std::string, TxOutput>> myUtxos;

	EVP_PKEY* getPrivateKey();
	Transaction buildTransaction(std::string receiverPublicKey, int amount);
	void scanUtxoSet(std::unordered_map<std::string, std::unordered_map<int, TxOutput>>& utxoset);
	void signTransaction(Transaction& tx);


private:
        EVP_PKEY* pkey;
};
