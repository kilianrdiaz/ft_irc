#pragma once

#include <iostream>
#include <string>

class Client
{
    private:
        int fd;
        std::string address;
        std::string recvBuffer;
        std::string nickname;
        std::string username;
        bool passOk;
        bool registered;

    public:
        Client() : fd(-1), passOk(false), registered(false) {};

        int getFd() {return fd;};
        std::string &getBuffer() {return recvBuffer;};
        std::string getNickname() {return nickname;};
        std::string getUsername() {return username;};
        bool getPassOk() {return passOk;};
        bool getRegistered() {return registered;};

        void setFd(int newFd) {fd = newFd;};
        void setIpAdd(std::string newAddress) {address = newAddress;};
        void setNickname(std::string newNick) {nickname = newNick;};
        void setUsername(std::string newUser) {username = newUser;};
        void setPassOk(bool value) {passOk = value;};
        void setRegistered(bool value) {registered = value;};
};