#include <sstream>
#include <sys/socket.h>
#include "Server.hpp"

Command Server::parseLine(const std::string &line)
{
    Command command;
    std::istringstream iss(line); 

    if (!(iss >> command.name)) // extrae palabra por palabra separadas por espacios
        return command;
    
    std::string rest;
    std::getline(iss, rest); // se coge el resto de la linea

    size_t start = rest.find_first_not_of(' '); // encuentra la primera pos que no sea un espacio
    if (start == std::string::npos) // si es todo espacios o no hay nada (ej.: QUIT), lo envia
        return command;

    rest = rest.substr(start);

    size_t pos = 0;
    while (pos < rest.size())
    {
        if (rest[pos] == ':') // último parametro, se coge entero sin tener en cuenta espacios
        {
            command.params.push_back(rest.substr(pos + 1));
            break;
        }

        size_t spacePos = rest.find(' ', pos); // encontrar proximo espacio
        if (spacePos == std::string::npos)
        {
            command.params.push_back(rest.substr(pos)); // si solo queda un argumento, lo añade
            break;
        }

        command.params.push_back(rest.substr(pos, spacePos - pos)); // añade parametros separados por espacios
        pos = spacePos + 1;

        while (pos < rest.size() && rest[pos] == ' ') // se salta espacios consecutivos
            pos++;
    }

    return command;
}

void Server::handleCommand(Client &client, const Command &command)
{
    if (command.name.empty())
        return;

    if (command.name == "PASS")
        cmdPass(client, command);
    else if (command.name == "NICK")
        cmdNick(client, command);
    else if (command.name == "USER")
        cmdUser(client, command);
    else if (command.name == "JOIN")
        cmdJoin(client, command);
    else if (command.name == "PRIVMSG")
        cmdPrivmsg(client, command);
    else if (command.name == "KICK")
        cmdKick(client, command);
    else if (command.name == "INVITE")
        cmdInvite(client, command);
    else if (command.name == "TOPIC")
        cmdTopic(client, command);
    else if (command.name == "MODE")
        cmdMode(client, command);
    else if (command.name == "PART")
        cmdPart(client, command);
    else if (command.name == "QUIT")
        cmdQuit(client, command);
    else
        std::cout << "Client <" << client.getFd() << "> Unknown command: " << command.name << std::endl;
}

void Server::sendToClient(Client &client, const std::string &msg)
{
    ssize_t sent;

    std::string fullMsg = msg + "\r\n";
    sent = send(client.getFd(), fullMsg.c_str(), fullMsg.size(), 0);

    // TODO : Puede que los mensajes no se envien completos, esto es solo un aviso,
    // puede que haya que implemenar una solución para los bytes que falten por mandar
    if (sent == -1)
        std::cout << "Client <" << client.getFd() << "> send() failed" << std::endl;
    else if (static_cast<size_t>(sent) < fullMsg.size())
        std::cout << "Client <" << client.getFd() << "> partial send (" << sent << "/" << fullMsg.size() << " bytes)" << std::endl;
}

// Helper para mensajes de error completos
void Server::sendReply(Client &client, const std::string &code, const std::string &message)
{
    std::string nick = client.getNickname().empty() ? "*" : client.getNickname();
    sendToClient(client, ":ircserv " + code + " " + nick + " " + message);
}

// Intenta registrar al usuario si todas las condiciones se cumplen
void Server::tryRegister(Client &client)
{
    if (client.getRegistered())
        return;

    if (client.getPassOk() && !client.getNickname().empty() && !client.getUsername().empty())
    {
        client.setRegistered(true);
        sendReply(client, "001", ":Welcome to the IRC server, " + client.getNickname());
    }
}

// Verificacion de contraseña del servidor para cada usuario
void Server::cmdPass(Client &client, const Command &command)
{
    if (client.getRegistered()) // Cliente ya registrado
    {
        sendReply(client, "462",  ":You may not reregister");
        return;
    }

    if (command.params.empty()) // Sin argumentos
    {
        sendReply(client, "461", "PASS :Not enough parameters");
        return;
    }

    if (command.params[0] != this->password) // Contraseña errónea
    {
        sendReply(client, "464", ":Password incorrect");
        return;
    }

    client.setPassOk(true);
    tryRegister(client); //Checkea si estan todas las condiciones válidas para que el cliente se pueda registrar
}

// Next TODO (Registro)
void Server::cmdNick(Client &client, const Command &command)
{
    (void)command;
    std::cout << "Client <" << client.getFd() << "> NICK (not implemented yet)" << std::endl;
}

// Next TODO (Registro)
void Server::cmdUser(Client &client, const Command &command)
{
    (void)command;
    std::cout << "Client <" << client.getFd() << "> USER (not implemented yet)" << std::endl;            
}

void Server::cmdJoin(Client &client, const Command &command)
{
    (void)command;
    std::cout << "Client <" << client.getFd() << "> JOIN (not implemented yet)" << std::endl;            
}

void Server::cmdPrivmsg(Client &client, const Command &command)
{
    (void)command;
    std::cout << "Client <" << client.getFd() << "> PRIVMSG (not implemented yet)" << std::endl;    
}

void Server::cmdKick(Client &client, const Command &command)
{
    (void)command;
    std::cout << "Client <" << client.getFd() << "> KICK (not implemented yet)" << std::endl;    
}

void Server::cmdInvite(Client &client, const Command &command)
{
    (void)command;
    std::cout << "Client <" << client.getFd() << "> INVITE (not implemented yet)" << std::endl;    
}

void Server::cmdTopic(Client &client, const Command &command)
{
    (void)command;
    std::cout << "Client <" << client.getFd() << "> TOPIC (not implemented yet)" << std::endl;    
}

void Server::cmdMode(Client &client, const Command &command)
{
    (void)command;
    std::cout << "Client <" << client.getFd() << "> MODE (not implemented yet)" << std::endl;    
}

void Server::cmdPart(Client &client, const Command &command)
{
    (void)command;
    std::cout << "Client <" << client.getFd() << "> PART (not implemented yet)" << std::endl;    
}

void Server::cmdQuit(Client &client, const Command &command)
{
    (void)command;
    std::cout << "Client <" << client.getFd() << "> QUIT (not implemented yet)" << std::endl;    
}