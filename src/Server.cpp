#include <sys/socket.h>   //-> for socket()
#include <sys/types.h>    //-> for socket()
#include <netinet/in.h>   //-> for sockaddr_in
#include <fcntl.h>         //-> for fcntl()
#include <unistd.h>        //-> for close()
#include <arpa/inet.h>    //-> for inet_ntoa()
#include <poll.h>          //-> for poll()
#include <csignal>         //-> for signal()
#include <cstring>         //-> for memset()
#include "Server.hpp"
#include "Command_handler.hpp"

bool Server::sig = false;

Server::Server(int port, std::string password) : port(port), password(password), serSocketFd(-1) {}

void Server::signalHandler(int signum)
{
    (void)signum;
    std::cout << std::endl << "Signal received" << std::endl;
    Server::sig = true;
}

void Server::serverInit()
{
    serSocket();

    std::cout << "Server <" << serSocketFd << "> Connected" << std::endl;
    std::cout << "Waiting to accept a connection...\n";

    while (Server::sig == false)
    {
        if ((poll(&fds[0], fds.size(), -1) == -1) && Server::sig == false)
            throw(std::runtime_error("poll() failed"));

        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == serSocketFd)
                    acceptNewClient();
                else
                {
                    receiveNewData(fds[i].fd);
                    break;
                }
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
    Client *newClient = new Client();
    struct sockaddr_in cliAddress;
    struct pollfd newPoll;
    socklen_t len = sizeof(cliAddress);

    int clifd = accept(serSocketFd, (sockaddr *)&cliAddress, &len); // aceptar al nuevo cliente, devuelve el nuevo socket del cliente
    if (clifd == -1)
    {
        std::cout << "accept() failed" << std::endl;
        delete newClient;
        return;
    }

    if (fcntl(clifd, F_SETFL, O_NONBLOCK) == -1) // hace que el nuevo socket sea no-bloqueante
    {
        std::cout << "fcntl() failed" << std::endl;
        delete newClient;
        close(clifd);
        return;
    }

    newPoll.fd = clifd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;

    newClient->setFd(clifd); // aplica el fd al cliente
    newClient->setIpAdd(inet_ntoa(cliAddress.sin_addr)); // convierte la IP a string y la añade
    clients[clifd] = newClient; // añade al nuevo cliente al map de clientes
    fds.push_back(newPoll); // añade el nuevo socket al pollfd

    std::cout << "Client <" << clifd << "> Connected" << std::endl;
}

void Server::receiveNewData(int fd)
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));

    ssize_t bytes = recv(fd, buf, sizeof(buf) - 1, 0); // recibe la data en buf

    if (bytes <= 0) // checkear si el cliente se ha desconectado
    {
        std::cout << "Client <" << fd << "> Disconnected" << std::endl;
        clearClient(fd);
        close(fd);
        return;
    }
    
    buf[bytes] = '\0';
    
    std::map<int, Client*>::iterator it = clients.find(fd);
    if (it == clients.end())
        return; // el cliente ya no existe, nada que hacer

    Client *client = it->second;
    std::string &buffer = client->getBuffer();
    buffer.append(buf, bytes);  // acumula lo recibido en el buffer del cliente

    size_t pos;
    while ((pos = buffer.find('\n')) != std::string::npos)
    {
        std::string line = buffer.substr(0, pos); // extrae una línea completa
        buffer.erase(0, pos + 1); // la elimina del buffer, incluyendo el '\n'

        if (!line.empty() && line[line.size() - 1] == '\r') // quita el '\r' si venía en formato \r\n
            line.erase(line.size() - 1);

        if (line.empty()) // ignora líneas vacías (p.ej. \r\n\r\n)
            continue;

        Command command = AbstractCommandHandler::parseLine(line);
        try {
            AbstractCommandHandler::executeCommand(*this, *client, command.name, command.params);
        }
        catch (const CommandException &e)
        {
            client->write(e.what());
        }
    }
}

void Server::closeFds()
{
    std::map<int, Client*>::iterator it = clients.begin();

    while (it != clients.end())
    {
        std::cout << "Client <" << it->first << "> Disconnected!" << std::endl;
        close(it->first);
        delete it->second;
        it++;
    }

    if (serSocketFd != -1)
    {
        std::cout << "Server <" << serSocketFd << "> Disconnected" << std::endl;
		close(serSocketFd);
    }
}

void Server::clearClient(int fd)
{
    for(size_t i = 0; i < fds.size(); i++)
    {
        if (fds[i].fd == fd)
        {
            fds.erase(fds.begin() + i);
            break;
        }
    }

    std::map<int, Client*>::iterator it = clients.find(fd);
    if (it != clients.end())
    {
        delete it->second; // liberar la memoria del cliente
        clients.erase(it); // eliminar la entrada del map
    }
}