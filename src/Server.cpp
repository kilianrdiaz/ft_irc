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

    while (Server::signal == false)
    {
        if ((poll(&fds[0], fds.size(), 1) == -1) && Server::signal == false)
            throw(std::runtime_error("poll() failed"));

        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == SerSocketFd)
                    acceptNewClient();
                else
                    receiveNewData(fds[i].fd);
            }
        }
    }
    closeFds();
}

void Server::serSocket()
{
    struct sockaddr_in address;
    struct pollfd newPoll;

    address.sin_family = AF_INET; // IPv4
    address.sin_port = htons(this->port); // se usa htons() para pasarlo a 'network byte order'
    address.sin_addr.s_addr = INADDR_ANY; //set the address to any local machine

    serSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serSocketFd == -1)
        throw(std::runtime_error("failed to create socket"));

    int en = 1;
    if (setsockopt(serSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) // configura opciones (SO_REUSEADDR → reusar puerto al reiniciar)
        throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
    if (fcntl(serSocketFd, F_SETFL, O_NONBLOCK) == -1) // hace que el socket sea no-bloqueante
        throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
    if (bind(serSocketFd, (struct sockaddr *)&address, sizeof(address)) == -1) // asocia el socket a IP + puerto
        throw(std::runtime_error("failed to bind socket"));
    if (listen(serSocketFd, SOMAXCONN) == -1) // empieza a escuchar conexiones entrantes, hace que el socket sea pasivo
        throw(std::runtime_error("listen() failed"));

    newPoll.fd = serSocketFd;
    newPoll.events = POLLIN; // qué quieres vigilar (POLLIN = datos listos para leer)
    newPoll.revents = 0; // qué pasó realmente (lo rellena poll())
    fds.push_back(newPoll);
}

void Server::acceptNewClient()
{
    Client newClient;
    struct sockaddr_in cliAddress;
    struct pollfd newPoll;
    socklen_t len = sizeof(cliAddress);

    int clifd = accept(SerSocketFd, (sockaddr *)&(cliadd), &len); // aceptar al nuevo cliente, devuelve el nuevo socket del cliente
    if (clifd == -1)
    {
        std::cout << "accept() failed" << std::endl;
        return;
    }

    if (fcntl(clifd, F_SETFL, O_NONBLOCK) == -1) // hace que el nuevo socket sea no-bloqueante
    {
        std::cout << "fcntl() failed" << std::endl;
        return;
    }

    newPoll.fd = clifd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;

    newClient.setFd(clifd); // aplica el fd al cliente
    newClient.setIpAdd(inet_ntoa((cliAddress.sin_addr))); // convierte la IP a string y la añade
    clients.push_back(newClient); // añade al nuevo cliente al vector de clientes
    fds.push_back(NewPoll); // añade el nuevo socket al pollfd

    std::cout << "Client <" << incofd << "> Connected" << std::endl;
}

void Server::receiveNewData(int fd)
{
    char buf[1024]; 
    memset(buff, 0, sizeof(buf));

    ssize_t bytes = recv(fd, buf, sizeof(buf) - 1, 0) // recibe la data en buf

    if (bytes < 0) // checkear si el cliente se ha desconectado
    {
        std::cout << "Client " << fd << "Disconnected" << std::endl;
        clearClient(fd);
        close(fd);
    }
    else
    {
        buf[bytes] = '\0';
        std::cout << "Client <" << fd "> Data: " << buf;

        // Añadir codigo para procesar la data (parse, check, authenticate, handle the command, etc...)
        // !!!!!
    }
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

void Server::clearClient(int fd)
{

}