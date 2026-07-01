#include <iostream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>

class Client
{
    private:
        int fd;
        std::string address;
    public:
        Client() {};
        int getFd() {return fd;};

        void setFd(int newFd) {fd = newFd;};
        void setIpAdd(std::string newAddress) {address = newAddress;};
}