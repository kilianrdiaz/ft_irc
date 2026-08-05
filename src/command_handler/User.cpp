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
        throw NotRegisteredException(_client.get_prefix());
    
    if (_client.getRegistered())
        throw AlreadyRegisteredException(_client.get_prefix());

    if (params.size() != 4)
        throw InvalidParametersException(_client.get_prefix(), "USER");

    std::string username = params[0];

    if (_client.getRegistered())
        throw AlreadyRegisteredException(_client.get_prefix());

    _client.setUsername(username);
    _client.setRealname(params[3]);
    _server.tryRegisterClient(_client);
}