#include <iostream>
#include <csignal>
#include <cstdlib>
#include "Server.hpp"

static bool isValidPort(const std::string &portStr, int &portOut)
{
    if(portStr.empty())
        return false;

    for (size_t i = 0; i < portStr.size(); i++)
    {
        if (!isdigit(portStr[i]))
            return false;
    }

    long port = std::atol(portStr.c_str());
    if (port < 1 || port > 65535)
        return false;

    portOut = static_cast<int>(port);
    return true;
}

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        std::cerr<< "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    int port;

    if (!isValidPort(argv[1], port))
    {
        std::cerr << "Error: invalid port. Must be a number between 1 and 65535.";
        return 1;
    }

    Server ser(port, argv[2]);

    try
    {
        signal(SIGINT, Server::signalHandler);
        signal(SIGQUIT, Server::signalHandler);
        ser.serverInit();
    }
    catch(const std::exception& e)
    {
        ser.closeFds();
        std::cerr << e.what() << std::endl;
    }
    std::cout << ">> Server closed <<" << std::endl;
}