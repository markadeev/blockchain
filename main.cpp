#include <iostream>
#include "block.h"

int main(){
        Block block1;
        Block block2;
        Block block3;

        block1.calculateHash();

        // print hash raw bytes in hex format
        for (auto c : block1.thisBlockHash){
                printf("%02x ", c);
        }

        return 0;
}

