//
// Created by Hanzhang Liu on 2019/2/25.
//

#ifndef MYBLOCKCHAIN_PEER_H
#define MYBLOCKCHAIN_PEER_H

#include "block.h"


class peer{
public:
    int peer_id;
    std::vector<BlockNode> blockchain;
    BlockNode currentNode=NULL;
    std::string peerAddress="127.0.0.1";
    int peerPort;
    bool reset = false;
    std::vector<uint16_t>  neighbor = {8080, 8081, 8082};

    //Constructor 
    peer(int index);
    peer(int index, std::string myAddress, uint16_t myPort,std::vector<uint16_t> nbr);

    //Core functions
    void genesis();
    void generateBlock();
    void store();
    bool validate(BlockNode rev_node);

    //Communication function
    void receive_chain(std::string chainString);
    void sendMessage(std::string message);

    //Aid functions
    void startComputing();
    std::string chainPack();

    //trivial function
    bool load();
    bool load(std::string str);
};



#endif //MYBLOCKCHAIN_PEER_H
