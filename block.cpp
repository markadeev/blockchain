#include <openssl/sha.h>
#include "block.h"

unsigned char * Block::calculateHash(){
	return SHA256(prevBlockHash, 32, thisBlockHash);
}
