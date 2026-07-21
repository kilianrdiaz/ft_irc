#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include "Client.hpp"

class Server
{
    private:
        int port;
        std::string password;
        int serSocketFd;
        static bool sig;
        std::map<int, Client*> clients;
        std::vector<struct pollfd> fds;

        public:
        Server(int port, std::string password);
        
        void serverInit();
        void serSocket();
        void acceptNewClient();
        void receiveNewData(int fd);
        std::string getPassword() const { return password; }
        std::map<int, Client*> &getClients() { return clients; }

        static void signalHandler(int signum);

        void closeFds();
        void clearClient(int fd);
};