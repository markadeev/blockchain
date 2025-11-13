#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <openssl/sha.h>
#include <iomanip>
using namespace std;
class Block {
	public:
		unsigned char prevBlockHash[32] = {0};
		time_t timestamp;
		int nonce = 0;
		unsigned char thisBlockHash[32];
		string MerkleRoot;
		vector<string> transactions;

		// calculate hash
		unsigned char * calculateHash(){
			return SHA256(prevBlockHash, 32, thisBlockHash);
		}
};
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
