# use g++ compiler
CXX = g++

# compiler flags: all warnings, extra warnings
# includes are in folders include, keys
CXXFLAGS = -Wall -Wextra -Iinclude -Ikeys

# linker links the following libraries
# lssl and lcrypto are needed for sha256 and ECDSA cryptography
LDFLAGS = -lssl -lcrypto

# source .cpp files
SRC = src/main.cpp src/block.cpp src/blockchain.cpp src/miner.cpp src/node.cpp src/transaction.cpp src/wallet.cpp

# for every .cpp file create .o object file
OBJ = $(SRC:.cpp = .o)

# output binary name
TARGET = main

# link step
# how to make the main executable from all the object files
$(TARGET): $(OBJ)
	# g++ src/main.o src/block.o ... -o main -lssl -lcrypto
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# compile step
# how to make an object file from the corresponding .cpp file
%.o: %.cpp
	# g++ -Wall -Wextra -Iinclude -Ikeys -c src/block.cpp -o src/block.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

# remove all compiled files
clean:
	rm -f $(OBJ) $(TARGET)

# tells Make that all and clean are not real files
.PHONY: all clean
