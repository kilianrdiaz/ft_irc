/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand_Handler.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 01:19:01 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/21 01:19:03 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command_handler.hpp"
#include "command_excepts.hpp"
#include "response.hpp"
#include <map>
#include <iostream>
#include <sstream>

/*
** Constructor / Destructor
*/

AbstractCommandHandler::AbstractCommandHandler(
    Server &server,
    Client &client)
    : _server(server), _client(client)
{
}

AbstractCommandHandler::AbstractCommandHandler(
    const AbstractCommandHandler &other)
    : _server(other._server), _client(other._client)
{
}

AbstractCommandHandler::~AbstractCommandHandler()
{
}

AbstractCommandHandler &AbstractCommandHandler::operator=(
    const AbstractCommandHandler &other)
{
    if (this != &other)
    {
        _server = other._server;
        _client = other._client;
    }
    return *this;
}

/*
** Handler functions
**
** Cada función crea el handler en el stack.
** No usamos new ni delete.
*/

static void handlePass(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    PassCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleNick(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    NickCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleUser(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    UserCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleCap(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    CapCommandHandler handler(server, client);
    handler.execute(params);
}

static void handlePing(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    PingCommandHandler handler(server, client);
    handler.execute(params);
}

// static void handleQuit(
//     Server &server,
//     Client &client,
//     const std::vector<std::string> &params)
// {
//     QuitCommandHandler handler(server, client);
//     handler.execute(params);
// }

// static void handlePrivmsg(
//     Server &server,
//     Client &client,
//     const std::vector<std::string> &params)
// {
//     PrivmsgCommandHandler handler(server, client);
//     handler.execute(params);
// }

static void handleJoin(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    JoinChannelCommandHandler handler(server, client);
    handler.execute(params);
}

static void handlePart(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    PartChannelCommandHandler handler(server, client);
    handler.execute(params);
}

// static void handleKick(
//     Server &server,
//     Client &client,
//     const std::vector<std::string> &params)
// {
//     KickChannelCommandHandler handler(server, client);
//     handler.execute(params);
// }

static void handleTopic(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    TopicCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleMode(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    ModeCommandHandler handler(server, client);
    handler.execute(params);
}

static std::map<std::string, HandlerExecutor> &getCommands()
{
    static std::map<std::string, HandlerExecutor> commands;

    if (commands.empty())
    {
        commands["PASS"] = &handlePass;
        commands["NICK"] = &handleNick;
        commands["USER"] = &handleUser;
        commands["PING"] = &handlePing;
        commands["CAP"] = &handleCap;
        // commands["QUIT"] = &handleQuit;
        // commands["PRIVMSG"] = &handlePrivmsg;
        commands["JOIN"] = &handleJoin;
        commands["PART"] = &handlePart;
        // commands["KICK"] = &handleKick;
        commands["TOPIC"] = &handleTopic;
        commands["MODE"] = &handleMode;
    }

    return (commands);
}

Command AbstractCommandHandler::parseLine(const std::string &line)
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

void AbstractCommandHandler::executeCommand(
    Server &server,
    Client &client,
    const std::string commandName,
    const std::vector<std::string> &params)
{
    std::map<std::string, HandlerExecutor> &commands = getCommands();

    std::map<std::string, HandlerExecutor>::iterator it =
        commands.find(commandName);

    if (it == commands.end())
    {
        throw CommandNotFoundException(
            client.getNickname(),
            commandName
        );
    }
    
    it->second(server, client, params);
}
