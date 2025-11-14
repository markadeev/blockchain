#include <ctime>
#include <string>
#include <vector>

class Block {
        public:
                unsigned char prevBlockHash[32];
                time_t timestamp;
                int nonce;
                unsigned char thisBlockHash[32];
                std::string MerkleRoot;
                std::vector<std::string> transactions;

                unsigned char * calculateHash();
};

