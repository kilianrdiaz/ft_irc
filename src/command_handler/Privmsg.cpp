#include "Command_handler.hpp"
#include "Channel.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

PrivmsgCommandHandler::PrivmsgCommandHandler(Server &server, Client &client) : AChannelCommandHandler(server, client)
{
}

PrivmsgCommandHandler::~PrivmsgCommandHandler()
{
}

void PrivmsgCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() != 2 || params[1].empty())
        throw InvalidParametersException(_client.get_prefix(), "PRIVMSG");

    std::string target = params[0];
    std::string message = params[1];

    if (!target.empty() && target[0] == '#')
    {
        Channel *channel = _server.getChannelByName(target);
        if (!channel)
            throw InvalidChannelException(_client.get_prefix(), target);

        if (!channel->isMember(_client.getFd()))
            throw CannotSendToChannelException(_client.get_prefix(), target);

        channel->broadcast(MSG_PRIVMSG(_client.get_prefix(), target, message), _client.getFd());
    }
    else
    {
        Client *targetClient = _server.searchNickname(target);

        if (targetClient == NULL)
            throw NoSuchNickException(_client.get_prefix(), target);

        targetClient->write(MSG_PRIVMSG(_client.get_prefix(), target, message));
    }
}