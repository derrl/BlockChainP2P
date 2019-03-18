//
// Created by Hanzhang Liu on 2019/2/25.
//

#ifndef MYBLOCKCHAIN_BLOCK_H
#define MYBLOCKCHAIN_BLOCK_H
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <ctime>
#include <vector>
#include "include/sha256.h"
#include <sstream>
#include "rpc/server.h"
#include "rpc/this_server.h"
#include "rpc/client.h"
#include "include/json.hpp"
#include <unistd.h>

#define DIFFICULTY  5
#define MaxChainLength  20
#define TimeToLive 5

using json = nlohmann::json;

class BlockNode
{

public:
    int index;
    std::time_t timestamp;
    std::string data;
    std::string Hash;
    std::string previousHash;
    int difficulty;
    int64_t nonce;
    int minerIndex;
    BlockNode(int Index);
    BlockNode(int Index, time_t time, std::string Data, std::string prevHash, std::string hashVal, int Difficulty, int minerIdx);
    BlockNode(json block_data);
    json toJSON();
};



#endif //MYBLOCKCHAIN_BLOCK_H
