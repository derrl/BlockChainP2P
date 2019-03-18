//
// Created by Hanzhang Liu on 2019/2/25.
//


#include "block.h"


///public:
///    int index;
///    std::time_t timestamp;
///    std::string data;
///    std::hash<std::string> hash_val;
///    std::hash<std::string> previousHash;
///    int difficulty;
///    int nonce;
///    BlockNode(int Index, time_t time, std::hash<std::string> hash_value, int Difficulty);
///};

BlockNode::BlockNode(int Index, time_t time, std::string Data, std::string prevHash, std::string hashVal, int Difficulty, int minerIdx){
        index = Index;
        timestamp = time;
        data = Data;
        Hash = hashVal;
        previousHash = prevHash;
        difficulty = Difficulty;
        minerIndex = minerIdx;
}

BlockNode::BlockNode(int Index) {
    index = Index;
}

json BlockNode::toJSON(){
    json j = {
            {"index", index},
            {"timestamp",timestamp},
            {"data",data},
            {"Hash",Hash},
            {"previousHash",previousHash},
            {"difficulty",difficulty},
            {"nonce",nonce},
            {"peerIndex",minerIndex}
    };
    return j;
}

BlockNode::BlockNode(json block_data) {
        index = block_data["index"];
        timestamp = block_data["timestamp"];
        data = block_data["data"];
        Hash = block_data["Hash"];
        previousHash =block_data["previousHash"];
        difficulty = block_data["difficulty"];
        nonce = block_data["nonce"];
        minerIndex = block_data["peerIndex"];

}


