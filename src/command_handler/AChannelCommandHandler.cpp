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

int AChannelCommandHandler::findMemberFdByNickname(
    Channel &channel,
    const std::string &nickname)
{
    std::map<int, ChannelMember> &members = channel.getMembers();
    std::map<int, ChannelMember>::iterator it = members.begin();

    while (it != members.end())
    {
        if (it->second.client->getNickname() == nickname)
            return it->first;
        it++;
    }
    return -1;
}

void AChannelCommandHandler::broadcast(
    Channel &channel,
    const std::string &message,
    int excludeFd)
{
    channel.broadcast(message, excludeFd);
}
