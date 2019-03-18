#pragma comment(lib, "ws2_32")
#include "peer.h"
#include <sys/stat.h>

int main(int argc, char * argv[]) {

    //config star topology connectivity
    std::vector<std::vector<uint16_t>> comm_map = {{8081,8082,8083,8084,8085,8086,8087,8088},
                                                   {8080},
                                                   {8080},
                                                   {8080},
                                                   {8080},
                                                   {8080},
                                                   {8080},
                                                   {8080},
                                                   {8080},
                                                   };
    for(int i = 1; i <= 8; i++){
        if(fork()==0){
            std::string k = "peer"+std::to_string(i);
            mkdir(k.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            chdir(k.c_str());
            peer* peerk =new  peer(i, "localhost", 8080+i,comm_map[i]);
            std::cout<< "Peer "<< i <<" created."<<std::endl;
            exit(0);
        }
    }

    //peer* peerk =new  peer(0, "localhost", 8080,{8081,8082,8083});
    mkdir("peer0", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    chdir("peer0");
    peer* peerk =new  peer(0, "localhost", 8080,comm_map[0]);
    std::cout<< "Peer "<< 0 <<" created."<<std::endl;

    return 0;
}

