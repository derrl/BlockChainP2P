//
// Created by Hanzhang Liu on 2019/2/25.
//

#include "peer.h"

void peer::genesis() {
    blockchain.push_back(BlockNode(0, time(0),"SpongeBob", "", sha256("SpongeBob"), 0, -1));
    std::cout<< "The first block Node is created." <<std::endl;
    std::cout<< "Index: " << blockchain[0].index <<std::endl;
    std::cout<< "hash: " << blockchain[0].Hash <<std::endl;
    std::cout<< "data: " << blockchain[0].data <<std::endl;
    std::cout<< "timestamp: " << blockchain[0].timestamp <<std::endl;
    std::cout<< "data: " << blockchain[0].data <<std::endl;
    std::cout<< "difficulty: " << blockchain[0].difficulty <<std::endl;
    std::cout<< "nonce: " << blockchain[0].nonce <<std::endl;
    std::cout<< "peerIndex: " << blockchain[0].minerIndex <<std::endl;
}

void peer::generateBlock() {
    label:
    std::cout<< "Start a new computing!"<<std::endl;

    currentNode = BlockNode(blockchain.back().index + 1, time(0), "haimianbaobao"+std::to_string(peerPort), blockchain.back().Hash, "", DIFFICULTY, peer_id);
    currentNode.nonce=-1;
    reset = false;

    char cstr[currentNode.difficulty+1];
    for(uint32_t i = 0; i < currentNode.difficulty; ++i){
        cstr[i] = '0';
    }
    cstr[currentNode.difficulty] = '\0';
    std::string str(cstr);

    //verify the result is valid;
    while(currentNode.Hash.substr(0, currentNode.difficulty) != str) {
        if (reset) {
            std::cout<< "Reset computing!!!"<<std::endl;
            goto label;
        } else {
            //calculation
            std::stringstream ss;
            ss << currentNode.index << currentNode.timestamp << currentNode.data << currentNode.nonce << currentNode.previousHash;
            currentNode.Hash=sha256(ss.str());
            //std::cout<< "Current try is :" << currentNode.Hash<<std::endl;
            currentNode.nonce++;
        }
    }
    currentNode.timestamp = time(0);
    sleep(1);
    currentNode.minerIndex = peer_id;
    //send new verify new chain and send
    if (validate(currentNode)){
        blockchain.push_back(currentNode);
        sendMessage(chainPack());

        //If reach the Max length of of the chain, store data and shutdown server.
        if(currentNode.index >= MaxChainLength){
            store();
            std::cout<< "Peer "<< peer_id<< ": Finished! Block chain stored. " <<std::endl;
            return;
        }
        goto label;
    }
    else{
        goto label;
    }
}


//Test a BlockNode whether is validate
bool peer::validate(BlockNode  rev_node) {
    bool isValid=true;

    std::cout<< "Get a new node to validate " <<std::endl;

    char cstr[rev_node.difficulty+1];
    for(uint32_t i = 0; i < rev_node.difficulty; ++i){
        cstr[i] = '0';
    }
    cstr[rev_node.difficulty] = '\0';
    std::string str(cstr);

    //Test the block received whether is valid
    if(isValid && rev_node.index <= blockchain.back().index) {
        //The block node is not valid, let the peer start a new calculation;
        isValid=false;
        std::cout<< "Test block index failed!" << std::endl;
    } else if(isValid && rev_node.timestamp <= blockchain.back().timestamp){
        isValid=false;
        std::cout<< "Test block timestamp failed!" << std::endl;
    } else if(isValid && rev_node.previousHash != blockchain.back().Hash){
        isValid=false;
        std::cout<< "Test block previous hash value failed!" << std::endl;
    } else if(isValid && rev_node.Hash.substr(0, rev_node.difficulty) != str ){
        isValid=false;
        std::cout<< "Test block hash value failed!" << std::endl;
    } else{
        std::cout<< " Validate successful!" <<std::endl;
    }
    return isValid;
}

//Store blockchain data to json file.
void peer::store() {
    std::ofstream bcFile;
    bcFile.open("blockchain.json");
    json bcJson;
    for(size_t i = 0; i < blockchain.size(); i++){
        bcJson[i] = blockchain[i].toJSON();
    }
    bcFile << bcJson;
    bcFile.close();
    return;
}

//Load the blockchian data from file
bool peer::load() {
    std::ifstream bcFile;
    bcFile.open("blockchain.json");
    json bcJson;
    bcJson << bcFile;
    if (bcJson.size()==0)
        return false;
    blockchain.clear();
    for(auto i : bcJson){
        blockchain.push_back(BlockNode(i));
    }
    return true;
}

std::string peer::chainPack(){
    json bcJson;
    for(size_t i = 0; i < blockchain.size(); i++){
        bcJson[i] = blockchain[i].toJSON();
    }
    //std::cout << "Chain to JSON string: "<< bcJson.dump() << std::endl;
    json Message;
    Message["blockchain"]= bcJson;
    Message["TTL"] = TimeToLive;
    return  Message.dump();
}

//load block chain data from message
bool peer::load(std::string mesg){
    std::cout<< "try to load current mesg" << std::endl;
    json Message;
    json bcJson;
    Message = json::parse(mesg);
    bcJson = Message["blockchain"];
    //std::cout<< "bcJson: "<<bcJson<<std::endl;

    //judge chain length
    if(bcJson.size() > blockchain.size()){
        blockchain.clear();
        for (size_t i = 0; i < bcJson.size(); i++){
            blockchain.push_back(bcJson[i]);
        }
        std::cout<< "message load successful "<<std::endl;
        return true;
    }
    else
        std::cout<< "message load failed "<<std::endl;
        return false;
}

//Only use for test
peer::peer(int index){
    peer_id=index;
}

peer::peer(int index, std::string myAddress, uint16_t myPort, std::vector<uint16_t> nbr) {
    peer_id = index;
    peerAddress = myAddress;
    peerPort = myPort;
    neighbor = nbr;

    //start rpc service
    rpc::server  peer(peerPort);
    std::cout<< "Start rpc service in port "<< peerPort<< std::endl;
    peer.bind("receiveChain",[this](std::string chainString){return this->receive_chain(chainString); } );
    constexpr size_t thread_count=4;
    peer.async_run(thread_count);
    //peer.run();

    //Create the genesis block
    if(peer_id == 0){
        genesis();
        std::cout<< "Genesis block created" << std::endl;
        sendMessage(chainPack());
    }
    startComputing();
}

void peer::startComputing(){
    while(blockchain.size() == 0){
        //waiting  a head to start computing
        sleep(1);
    }
    generateBlock();
}

void peer::sendMessage(std::string message) {
    //decrease TTL
    json Message = json::parse(message);
    Message["TTL"] = ((int) Message["TTL"]) - 1;
    std::cout<< "current TTL is " << Message["TTL"] << ". In peer "<< peer_id<< std::endl;
    std::string mesg = Message.dump();

    //Send mesg to all neighbors
    for (int i = 0; i < neighbor.size();i++) {
        std::cout << "Send message to port " << neighbor[i] << std::endl;
        rpc::client seblk(peerAddress, neighbor[i]);
        seblk.set_timeout(5000);
        //std::cout<< "Current timeout is "<< seblk.get_timeout() << std::endl;
        //std::cout<< "send chain: " << mesg<< std::endl;
        seblk.call("receiveChain", mesg);
    }

}

void peer::receive_chain(std::string chainString){
    //unpack Message
    //std::cout<< "received chain: " << chainString<< std::endl;
    if(load(chainString)) {
        //Reset current computing
        if(blockchain.size() <= MaxChainLength) {
            store();
        }
        if(blockchain.size() > MaxChainLength){
            std::cout<< "Peer "<< peer_id<< ": Finished! Block chain stored. " <<std::endl;
            exit(0);
        }

        if(blockchain.size()!=1){
            std::cout << "Reset current computing. Peer "<< peer_id<< std::endl;
            reset = true;
        }
        //Send this block chain to its neighbors
        json Message = json::parse(chainString);
        if (Message["TTL"] > 1) {
            sendMessage(chainString);
        }
    }
    return;
}

