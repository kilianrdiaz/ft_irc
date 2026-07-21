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
        Client();
        Client(int socket_fd);
        Client(const Client &other);
        ~Client();
        Client &operator=(const Client &other);

        int getFd();
        std::string &getBuffer();
        std::string getNickname();
        std::string getUsername();
        bool getPassOk();
        bool getRegistered();

        void setFd(int newFd);
        void setIpAdd(std::string newAddress);
        void setNickname(std::string newNick);
        void setUsername(std::string newUser);
        void setPassOk(bool value);
        void setRegistered(bool value);
        void write (const std::string &message);
        void reply(const std::string &code, const std::string &message);
};