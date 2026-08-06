#include <sys/socket.h>   //-> for socket()
#include <sys/types.h>    //-> for socket()
#include <netinet/in.h>   //-> for sockaddr_in
#include <fcntl.h>         //-> for fcntl()
#include <unistd.h>        //-> for close()
#include <arpa/inet.h>    //-> for inet_ntoa()
#include <poll.h>          //-> for poll()
#include <csignal>         //-> for signal()
#include <cstring>         //-> for memset()
#include <ctime>           //-> for time(), localtime(), strftime()
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
        log(EVENT, "Client <" + ::toStr(it->first) + "> disconnected (server shutdown)");
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
        log(INFO, "Server socket <" + ::toStr(serSocketFd) + "> closed");
        close(serSocketFd);
    }
}

void Server::signalHandler(int signum)
{
    (void)signum;
    log(WARN, "Signal received, shutting down...");
    Server::sig = true;
}

void Server::serverInit()
{
    serSocket();

    log(INFO, "Server socket <" + toStr(serSocketFd) + "> ready");
    log(INFO, "Waiting for connections...");

    while (Server::sig == false)
    {
        // Antes de cada poll(), decidimos a quién vigilar también para escritura
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].fd == serSocketFd)
                continue;

            Client *c = getClientByFd(fds[i].fd);
            if (c != NULL && c->hasPendingWrite())
                fds[i].events = POLLIN | POLLOUT;
            else
                fds[i].events = POLLIN;
        }

        if ((poll(&fds[0], fds.size(), -1) == -1) && Server::sig == false)
        {
            log(ERROR, "poll() failed");
            throw(std::runtime_error("poll() failed"));
        }

        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].fd == serSocketFd)
            {
                if (fds[i].revents & POLLIN)
                    acceptNewClient();
                continue;
            }

            Client *c = getClientByFd(fds[i].fd);
            if (c == NULL)
                continue;

            if (fds[i].revents & POLLOUT)
                c->flushSend();

            if (fds[i].revents & POLLIN)
            {
                receiveNewData(fds[i].fd);
                break; // fds pudo mutar (clearClient); reevaluamos en el siguiente poll()
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
        log(ERROR, "failed to create socket");
        throw(std::runtime_error("failed to create socket"));
    }

    int en = 1;
    if (setsockopt(serSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
    {
        log(ERROR, "failed to set option (SO_REUSEADDR) on socket");
        throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
    }
    if (fcntl(serSocketFd, F_SETFL, O_NONBLOCK) == -1)
    {
        log(ERROR, "failed to set option (O_NONBLOCK) on socket");
        throw(std::runtime_error("failed to set option (O_NONBLOCK) on socket"));
    }
    if (bind(serSocketFd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        log(ERROR, "failed to bind socket");
        throw(std::runtime_error("failed to bind socket"));
    }
    if (listen(serSocketFd, SOMAXCONN) == -1)
    {
        log(ERROR, "failed to listen on socket");
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
    struct sockaddr_in serverAddress;
    struct pollfd newPoll;
    socklen_t cliLen = sizeof(cliAddress);
    socklen_t serverLen = sizeof(serverAddress);

    int clifd = accept(serSocketFd, (sockaddr *)&cliAddress, &cliLen);
    if (clifd == -1)
    {
        log(ERROR, "accept() failed");
        delete newClient;
        return;
    }

    if (fcntl(clifd, F_SETFL, O_NONBLOCK) == -1)
    {
        log(ERROR, "fcntl() failed");
        delete newClient;
        close(clifd);
        return;
    }

    newPoll.fd = clifd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;

    newClient->setFd(clifd);

    if (getsockname(clifd, (sockaddr *)&serverAddress, &serverLen) == 0)
    {
        newClient->setHostName(inet_ntoa(cliAddress.sin_addr));
        newClient->setServername(inet_ntoa(serverAddress.sin_addr));
    }

    clients[clifd] = newClient;
    fds.push_back(newPoll);

    log(EVENT, "Client <" + ::toStr(clifd) + "> connected from " + newClient->getHost());
}

void Server::receiveNewData(int fd)
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));

    ssize_t bytes = recv(fd, buf, sizeof(buf) - 1, 0);

    if (bytes <= 0)
    {
        log(EVENT, "Client <" + ::toStr(fd) + "> disconnected");

        Client *disconnecting = getClientByFd(fd);
        if (disconnecting != NULL)
            removeClientFromChannels(*disconnecting, MSG_QUIT(disconnecting->get_prefix(), "Connection closed"));

        clearClient(fd);
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

        log(EVENT, "Client <" + ::toStr(fd) + "> (" + client->getNickname() + ") -> " + command.name);

        try
        {
            AbstractCommandHandler::executeCommand(*this, *client, command.name, command.params);
        }
        catch (const QuitException &e)
        {
            clearClient(fd);
            return;
        }
        catch (const CommandException &e)
        {
            log(WARN, "Client <" + ::toStr(fd) + "> error on " + command.name + ": " + e.what());
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
        log(EVENT, "Client <" + ::toStr(client.getFd()) + "> registered as " + client.getNickname());
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
    std::string serverReply;

    if (!client)
        return;
    serverReply = ":" + client->getServername() + " " + message;
    client->write(serverReply);
}

std::string Server::getPassword() const
{
    return password;
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
    log(EVENT, "Channel " + channel->getName() + " created");
}

void Server::removeChannel(const std::string &channelName)
{
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (it != channels.end())
    {
        log(EVENT, "Channel " + channelName + " destroyed (empty)");
        delete it->second;
        channels.erase(it);
    }
}

void Server::clearClient(int fd)
{
    //removeClientFromChannels(*getClientByFd(fd), "Connection closed");
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
    close(fd);
}

void Server::removeClientFromChannels(Client &client, const std::string &message)
{
    std::map<std::string, Channel*>::iterator it = channels.begin();

    while (it != channels.end())
    {
        Channel *channel = it->second;

        if (channel->isMember(client.getFd()))
        {
            channel->broadcast(message, client.getFd());
            channel->removeMember(client.getFd());
        }
        ++it;
    }
}
