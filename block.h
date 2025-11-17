#include <ctime>
#include <string>
#include <vector>

class Block {
        public:
                std::string prevBlockHash;
                time_t timestamp;
                long long nonce;
                std::string thisBlockHash;
                std::string MerkleRoot;
                std::vector<std::string> transactions;

		Block(std::string prevBlockHash);
		
		std::string calculateHash();
		void mineBlock();

};

