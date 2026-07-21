/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 01:52:33 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/21 01:52:35 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command_handler.hpp"
#include "response.hpp"

PassCommandHandler::PassCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

PassCommandHandler::~PassCommandHandler()
{
}

void PassCommandHandler::execute(const std::vector<std::string> &params)
{
    if (_client.getPassOk())
        throw AlreadyPassedException(_client.getNickname());
    if (params.size() != 1)
        throw InvalidParametersException(_client.getNickname(), "PASS");

    std::string password = params[0];
    if (password != _server.getPassword())
        throw InvalidPasswordException(_client.getNickname());

    _client.setPassOk(true);
}