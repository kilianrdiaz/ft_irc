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
#include "response.hpp"

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
    if (_client.getRegistered())
        throw AlreadyRegisteredException(_client.getNickname());

    if (params.size() != 1)
        throw InvalidParametersException(_client.getNickname(), "NICK");

    std::string nickname = params[0];
    if (nickname.empty())
        throw InvalidNicknameException(_client.getNickname());

    _client.setNickname(nickname);
    _client.tryRegister();
}
