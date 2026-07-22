/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AChannelCommandHandler.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 02:00:50 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/22 02:00:53 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command_handler.hpp"
#include "command_excepts.hpp"
#include "response.hpp"
#include <sstream>


AChannelCommandHandler::AChannelCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

AChannelCommandHandler::~AChannelCommandHandler()
{
}

std::vector<std::string> AChannelCommandHandler::parseChannelList(
    const std::string &channelList)
{
    std::vector<std::string> channels;
    std::stringstream ss(channelList);
    std::string channel;

    while (std::getline(ss, channel, ','))
    {
        if (!channel.empty())
            channels.push_back(channel);
    }

    return channels;
}

Channel *AChannelCommandHandler::getChannelByName(
    const std::string &channelName)
{
    std::map<std::string, Channel*> &channels = _server.getChannels();
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);

    // si no empiezan con #, no es un canal válido
    if (channelName.empty() || channelName[0] != '#')
        throw InvalidChannelException(_client.getNickname(), channelName);

    if (it == channels.end())
        throw InvalidChannelException(_client.getNickname(), channelName);

    return it->second;
}