#include <iostream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>

class Server
{
    private:
        int port;
        int serSocketFd;
        static bool signal;
        std::map<int, Client*> clients;
        std::vector<struct pollfd> fds;
    public:
        Server() {serSockerFd = -1;};

        void serverInit();
        void serSocket();
        void acceptNewClient();
        void receiveNewData(int fd);

        static void signalHandler(int signum);

        void closeFds();
        void clearClients(int fd);
}