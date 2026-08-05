/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 02:42:13 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/21 02:42:15 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command_handler.hpp"
#include "command_excepts.hpp"
#include "response.hpp"
#include "Channel.hpp"

NickCommandHandler::NickCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

NickCommandHandler::~NickCommandHandler()
{
}

void NickCommandHandler::execute(const std::vector<std::string> &params)
{
    if (!_client.getPassOk())
        throw NotRegisteredException(_client.getNickname());

    if (params.size() != 1)
        throw InvalidParametersException(_client.getNickname(), "NICK");

    std::string nickname = params[0];
    if (nickname.empty())
        throw InvalidNicknameException(_client.getNickname());

    if (_server.searchNickname(nickname, _client.getFd()))
        throw AlreadyExistNicknameException(_client.getNickname(), nickname);

    bool wasRegistered = _client.getRegistered();
    std::string oldPrefix = _client.get_prefix();

    _client.setNickname(nickname);

    if (wasRegistered)
    {
        std::string nickMsg = MSG_NICK(oldPrefix, nickname);
        _client.write(nickMsg);

        std::map<std::string, Channel*> allChannels = _server.getChannels();
        std::map<std::string, Channel*>::iterator it = allChannels.begin();
        while (it != allChannels.end())
        {
            if (it->second->isMember(_client.getFd()))
                it->second->broadcast(nickMsg, _client.getFd());
            ++it;
        }
    }

    _server.tryRegisterClient(_client);
}