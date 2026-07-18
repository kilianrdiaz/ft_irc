#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include "Client.hpp"
#include "Command.hpp"

class Server
{
    private:
        int port;
        std::string password;
        int serSocketFd;
        static bool sig;
        std::map<int, Client*> clients;
        std::vector<struct pollfd> fds;

        Command parseLine(const std::string &line);
        void handleCommand(Client &client, const Command &command);

        void sendToClient(Client &client, const std::string &message);
        void sendReply(Client &client, const std::string &code, const std::string &message);
        void tryRegister(Client &client);

        void cmdPass(Client &client, const Command &command);
        void cmdNick(Client &client, const Command &command);
        void cmdUser(Client &client, const Command &command);
        void cmdJoin(Client &client, const Command &command);
        void cmdPrivmsg(Client &client, const Command &command);
        void cmdKick(Client &client, const Command &command);
        void cmdInvite(Client &client, const Command &command);
        void cmdTopic(Client &client, const Command &command);
        void cmdMode(Client &client, const Command &command);
        void cmdPart(Client &client, const Command &command);
        void cmdQuit(Client &client, const Command &command);

    public:
        Server(int port, std::string password);

        void serverInit();
        void serSocket();
        void acceptNewClient();
        void receiveNewData(int fd);

        static void signalHandler(int signum);

        void closeFds();
        void clearClient(int fd);
};