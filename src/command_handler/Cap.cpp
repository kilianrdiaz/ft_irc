/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 16:24:13 by alejhern          #+#    #+#             */
/*   Updated: 2026/07/21 16:24:16 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command_handler.hpp"
#include "response.hpp"

CapCommandHandler::CapCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

CapCommandHandler::~CapCommandHandler()
{
}

void CapCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() < 1)
        throw InvalidParametersException(_client.getNickname(), "CAP");

    std::string subcommand = params[0];
    if (subcommand == "LS")
    {
        _client.reply("CAP * LS :multi-prefix");
    }
    else if (subcommand == "REQ")
    {
        _client.reply("CAP * ACK :multi-prefix");
    }
    else
    {
        throw InvalidParametersException(_client.getNickname(), "CAP");
    }
}