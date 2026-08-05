/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 03:11:19 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/21 16:23:00 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command_handler.hpp"
#include "command_excepts.hpp"

PingCommandHandler::PingCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

PingCommandHandler::~PingCommandHandler()
{
}

void PingCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() != 1)
        throw InvalidParametersException(_client.get_prefix(), "PING");

    if (!_client.getPassOk())
        throw NotRegisteredException(_client.get_prefix());

    std::string pingMessage = params[0];
    if (pingMessage.empty())
        throw InvalidPingException(_client.get_prefix());

    _client.write("PONG :" + pingMessage);
}
