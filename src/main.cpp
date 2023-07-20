#include "server.cpp"
#include <iostream>

int main(int argc, char* argv[]){
    std::locale::global(std::locale(""));
    if(argc < 3) {
        std::cout << "Count of parameters is incorrect! Please enter sever Port and BULK SIZE" << std::endl;
        return 1;
    }
    const size_t port = std::stoi(argv[1]);
    const size_t sz = std::stoi(argv[2]);

    Server server(port, sz);

    return 0;
}