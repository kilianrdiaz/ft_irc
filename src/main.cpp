#include <iostream>
#include <csignal>
#include <cstdlib>
#include "Server.hpp"
#include "response.hpp"

static bool isValidPort(const std::string &portStr, int &portOut)
{
    if (portStr.empty())
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
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    int port;
    if (!isValidPort(argv[1], port))
    {
        std::cerr << "Error: invalid port. Must be a number between 1 and 65535." << std::endl;
        return 1;
    }

    std::cout << COL_EVENT 
        "  ______ _______     _____ _____   _____ \n"
        " |  ____|__   __|   |_   _|  __ \\ / ____|\n"
        " | |__     | |  ___   | | | |__) | |     \n"
        " |  __|    | | |___|  | | |  _  /| |     \n"
        " | |       | |       _| |_| | \\ \\| |____ \n"
        " |_|       |_|      |_____|_|  \\_\\\\_____|\n"
        << COL_RESET;

    std::cout << COL_INFO << " ------------------------------------------" << COL_RESET << "\n";
    std::cout << "  \033[1mInternet Relay Chat Server\033[0m — C++98\n";
    std::cout << "  Port:     " << COL_WARN << port << COL_RESET << "\n";
    std::cout << "  Password: " << COL_WARN << "********" << COL_RESET << "\n";
    std::cout << COL_INFO << " ------------------------------------------" << COL_RESET << "\n" << std::endl;

    Server ser(port, argv[2]);

    try
    {
        signal(SIGINT, Server::signalHandler);
        signal(SIGQUIT, Server::signalHandler);
        ser.serverInit();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::cout << COL_EVENT << ">> Server closed <<" << COL_RESET << std::endl;
}