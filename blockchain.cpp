#include <iostream>
#include <string>
#include <ctime>
#include <vector>
using namespace std;
class Block {
	public:
		string prevBlockHash;
		time_t timestamp;
		int nonce;
		string thisBlockHash;
		string MerkleRoot;
		vector<string> transactions;
};
int main(){
	Block block1;
	Block block2;
	Block block3;
	block2.prevBlockHash = block1.thisBlockHash;
	block3.prevBlockHash = block2.thisBlockHash;

	return 0;
}
