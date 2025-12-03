#include <vector>
#include "block.h"

class Blockchain {
        public:
                std::vector<Block> chain;

                void createGenesisBlock();
                Block& getLatestBlock();
                void addBlock(Block newblock);
};

