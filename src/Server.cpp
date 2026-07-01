#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()
#include "Server.hpp"

bool Server::signal = false;

static void Server::signalHandler(int signum)
{
    (void)signum;
    std::cout << std::endl << "Signal received" << std::endl;
    Server::signal = true;
}

void Server::serverInit()
{
    this->port = 4242;
    serSocket();

    std::cout << "Server <" << sesrSocketFd << "> Connected" << std::endl;
    std::cout << "Waiting to accept a connection...\n";
}

void Server::serSocket()
{
    struct sockaddr_in address;
    struct pollfd newPoll;

    address.sin_family = AF_INET; // IPv4
    address.sin_port = htons(this->port);
    address.sin_addr.s_addr = INADDR_ANY; //set the address to any local machine

    serSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serSocketFd == -1)
        throw(std::runtime_error("failed to create socket"));

    int en = 1;
    if (setsockopt(serSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) // configura opciones (SO_REUSEADDR → reusar puerto al reiniciar)
        throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
    if (fcntl(serSocketFd, F_SETFL, O_NONBLOCK) == -1)
        throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
    if (bind(serSocketFd, (struct sockaddr *)&address, sizeof(address)) == -1) // asocia el socket a IP + puerto
        throw(std::runtime_error("failed to bind socket"));
    if (listen(serSocketFd, SOMAXCONN) == -1) // empieza a escuchar conexiones entrantes
        throw(std::runtime_error("listen() failed"));

    newPoll.fd = serSocketFd;
    newPoll.events = POLLIN; // qué quieres vigilar (POLLIN = datos listos para leer)
    newPoll.revents = 0; // qué pasó realmente (lo rellena poll())
    fds.push_back(newPoll);
}

void Server::acceptNewClient()
{

}

void Server::receiveNewData(int fd)
{

}

void Server::closeFds()
{
    std::map<int, Client*>::iterator it = clients.begin();

    while (it != clients.end())
    {
        std::cout << "Client" << it->first << "Disconnected!" << std::endl;
        close(it->first);
        delete it->second;
        it++;
    }

    if (serSocketFd != -1)
    {
        std::cout << "Server <" << SerSocketFd << "> Disconnected" << std::endl;
		close(SerSocketFd);
    }
}

void Server::clearClients(int fd)
{

}