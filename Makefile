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
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# compile step
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean build files
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
