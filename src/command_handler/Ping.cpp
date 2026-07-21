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

PingCommandHandler::PingCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

PingCommandHandler::~PingCommandHandler()
{
}

void PingCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() != 1)
        throw InvalidParametersException(_client.getNickname(), "PING");

    if (!_client.getPassOk())
        throw NotRegisteredException(_client.getNickname());

    std::string pingMessage = params[0];
    if (pingMessage.empty())
        throw InvalidPingException(_client.getNickname());

    _client.reply("PONG :" + pingMessage);
}
