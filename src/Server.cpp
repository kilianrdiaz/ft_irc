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
#include "command_excepts.hpp"
#include "response.hpp"

bool Server::sig = false;

Server::Server(int port, std::string password) : port(port), password(password), serSocketFd(-1) {}

Server::Server(const Server &other)
    : port(other.port), password(other.password), serSocketFd(other.serSocketFd),
      clients(other.clients), fds(other.fds), channels(other.channels) {}

Server &Server::operator=(const Server &other)
{
    if (this != &other)
    {
        port = other.port;
        password = other.password;
        serSocketFd = other.serSocketFd;
        clients = other.clients;
        fds = other.fds;
        channels = other.channels;
    }
    return *this;
}

Server::~Server()
{
    std::map<int, Client*>::iterator it = clients.begin();

    while (it != clients.end())
    {
        std::cout << COL_EVENT << "Client <" << it->first << "> Disconnected!" << COL_RESET << std::endl;
        close(it->first);
        delete it->second;
        it++;
    }

    std::map<std::string, Channel*>::iterator cit = channels.begin();

    while (cit != channels.end())
    {
        delete cit->second;
        cit++;
    }

    if (serSocketFd != -1)
    {
        std::cout << COL_INFO << "Server <" << serSocketFd << "> Disconnected" << COL_RESET << std::endl;
        close(serSocketFd);
    }
}

void Server::signalHandler(int signum)
{
    (void)signum;
    std::cout << COL_WARN << "Signal received" << COL_RESET << std::endl;
    Server::sig = true;
}

void Server::serverInit()
{
    serSocket();

    std::cout << COL_INFO << "Server <" << serSocketFd << "> Connected" << COL_RESET << std::endl;
    std::cout << COL_INFO << "Waiting to accept a connection..." << COL_RESET << "\n";

    while (Server::sig == false)
    {
        if ((poll(&fds[0], fds.size(), -1) == -1) && Server::sig == false)
        {
            std::cout << COL_ERROR << "poll() failed" << COL_RESET << std::endl;
            throw(std::runtime_error("poll() failed"));
        }

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
    {
        std::cout << COL_ERROR << "failed to create socket" << COL_RESET << std::endl;
        throw(std::runtime_error("failed to create socket"));
    }

    int en = 1;
    if (setsockopt(serSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
    {
        std::cout << COL_ERROR << "failed to set option (SO_REUSEADDR) on socket" << COL_RESET << std::endl;
        throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
    }
    if (fcntl(serSocketFd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cout << COL_ERROR << "failed to set option (O_NONBLOCK) on socket" << COL_RESET << std::endl;
        throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
    }
    if (bind(serSocketFd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        std::cout << COL_ERROR << "failed to bind socket" << COL_RESET << std::endl;
        throw(std::runtime_error("failed to bind socket"));
    }
    if (listen(serSocketFd, SOMAXCONN) == -1)
    {
        std::cout << COL_ERROR << "listen() failed" << COL_RESET << std::endl;
        throw(std::runtime_error("listen() failed"));
    }

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

    int clifd = accept(serSocketFd, (sockaddr *)&cliAddress, &len);
    if (clifd == -1)
    {
        std::cout << COL_ERROR << "accept() failed" << COL_RESET << std::endl;
        delete newClient;
        return;
    }

    if (fcntl(clifd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cout << COL_ERROR << "fcntl() failed" << COL_RESET << std::endl;
        delete newClient;
        close(clifd);
        return;
    }

    newPoll.fd = clifd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;

    newClient->setFd(clifd);
    newClient->setHostName(inet_ntoa(cliAddress.sin_addr));
    clients[clifd] = newClient;
    fds.push_back(newPoll);

    std::cout << COL_EVENT << "Client <" << clifd << "> Connected" << COL_RESET << std::endl;
}

void Server::receiveNewData(int fd)
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));

    ssize_t bytes = recv(fd, buf, sizeof(buf) - 1, 0);

    if (bytes <= 0)
    {
        std::cout << COL_EVENT << "Client <" << fd << "> Disconnected" << COL_RESET << std::endl;

        Client *disconnectedClient = getClientByFd(fd);
        if (disconnectedClient != NULL)
            disconnectClient(*disconnectedClient, "Connection closed");

        clearClient(fd);
        close(fd);
        return;
    }

    buf[bytes] = '\0';

    Client *client = getClientByFd(fd);
    if (client == NULL)
        return;

    std::string &buffer = client->getBuffer();
    buffer.append(buf, bytes);

    size_t pos;
    while ((pos = buffer.find('\n')) != std::string::npos)
    {
        std::string line = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);

        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        if (line.empty())
            continue;

        Command command = AbstractCommandHandler::parseLine(line);

        std::cout << COL_EVENT << "Client <" << fd << "> (" << client->getNickname()
                   << ") -> " << command.name << COL_RESET << std::endl;

        try
        {
            AbstractCommandHandler::executeCommand(*this, *client, command.name, command.params);
        }
        catch (const QuitException &e)
        {
            clearClient(fd);
            close(fd);
            return;
        }
        catch (const CommandException &e)
        {
            std::cout << COL_WARN << "Client <" << fd << "> error on " << command.name
                       << ": " << e.what() << COL_RESET << std::endl;
            this->replyToClient(fd, e.what());
            return;
        }
    }
}

void Server::tryRegisterClient(Client &client)
{
    if (client.getRegistered())
        return;

    bool hasRealNickname = !client.getNickname().empty() && client.getNickname() != "*";

    if (client.getPassOk() && hasRealNickname && !client.getUsername().empty())
    {
        client.setRegistered(true);
        std::cout << COL_EVENT << "Client <" << client.getFd() << "> registered as "
                   << client.getNickname() << COL_RESET << std::endl;
        this->replyToClient(client.getFd(), RPL_WELCOME(client.getNickname()));
    }
}

Client *Server::searchNickname(const std::string &nickname, int excludeFd)
{
    std::map<int, Client*>::iterator it = clients.begin();

    while (it != clients.end())
    {
        if (it->first != excludeFd && it->second->getNickname() == nickname)
            return it->second;
        it++;
    }
    return NULL;
}

Client *Server::getClientByFd(int fd)
{
    std::map<int, Client*>::iterator it = clients.find(fd);
    if (it == clients.end())
        return NULL;
    return it->second;
}

void Server::replyToClient(int fd, const std::string &message)
{
    Client *client = getClientByFd(fd);
    if (client != NULL)
        client->write(message);
}

std::map<int, Client*> Server::getClients()
{
    return clients;
}

std::map<std::string, Channel*> Server::getChannels()
{
    return channels;
}

Channel *Server::getChannelByName(const std::string &channelName)
{
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (it == channels.end())
        return NULL;
    return it->second;
}

void Server::addChannel(Channel *channel)
{
    channels[channel->getName()] = channel;
}

void Server::removeChannel(const std::string &channelName)
{
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (it != channels.end())
    {
        delete it->second;
        channels.erase(it);
    }
}

void Server::clearClient(int fd)
{
    for (size_t i = 0; i < fds.size(); i++)
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
        delete it->second;
        clients.erase(it);
    }
}

void Server::removeClientFromChannels(Client &client, const std::string &message)
{
    std::map<std::string, Channel*>::iterator it = channels.begin();

    while (it != channels.end())
    {
        Channel *channel = it->second;

        if (channel->isMember(client.getFd()))
        {
            channel->removeMember(client.getFd());
            channel->broadcast(message);

            if (channel->memberCount() == 0)
            {
                delete channel;
                std::map<std::string, Channel*>::iterator toErase = it;
                ++it;
                channels.erase(toErase);
                continue;
            }
        }
        ++it;
    }
}

void Server::disconnectClient(Client &client, const std::string &reason)
{
    std::string quitMsg = MSG_QUIT(client.get_prefix(), reason);
    removeClientFromChannels(client, quitMsg);
}