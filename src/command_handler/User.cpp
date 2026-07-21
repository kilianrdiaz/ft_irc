/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 02:15:16 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/21 02:15:18 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command_handler.hpp"
#include "response.hpp"


UserCommandHandler::UserCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

UserCommandHandler::~UserCommandHandler()
{
}

void UserCommandHandler::execute(const std::vector<std::string> &params)
{
    if (!_client.getPassOk())
        throw NotRegisteredException(_client.getNickname());
    
    if (_client.getRegistered())
        throw AlreadyRegisteredException(_client.getNickname());

    if (params.size() != 4)
        throw InvalidParametersException(_client.getNickname(), "USER");

    std::string username = params[0];
    if (username.empty())
        throw InvalidUsernameException(_client.getNickname());

    if (_client.getRegistered())
        throw AlreadyRegisteredException(_client.getNickname());

    _client.setUsername(username);
    _client.setRealname(params[3]);
    _client.setRegistered(true);
    _client.tryRegister();
}