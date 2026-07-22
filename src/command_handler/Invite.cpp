#include "Command_handler.hpp"
#include "Channel.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

InviteCommandHandler::InviteCommandHandler(Server &server, Client &client) : AChannelCommandHandler(server, client)
{
}

InviteCommandHandler::~InviteCommandHandler()
{
}

void InviteCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() != 2)
        throw InvalidParametersException(_client.getNickname(), "INVITE");

    std::string targetNick = params[0];
    std::string channelName = params[1];

    std::map<std::string, Channel*> &channels = _server.getChannels();
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);

    if (it == channels.end())
        throw InvalidChannelException(_client.getNickname(), channelName);

    Channel *channel = it->second;

    if (!channel->isMember(_client.getFd()))
        throw NotInChannelException(_client.getNickname(), channelName);

    if (channel->getInviteOnly() && !channel->isOperator(_client.getFd()))
        throw NotPrivilegedException(_client.getNickname(), channelName);

    Client *targetClient = _server.searchNickname(targetNick);

    if (targetClient == NULL)
        throw NoSuchNickException(_client.getNickname(), targetNick);

    if (channel->isMember(targetClient->getFd()))
        throw AlreadyInChannelException(targetNick, channelName);

    channel->invite(targetClient->getFd());

    _client.write(RPL_INVITING(_client.getNickname(), targetNick, channelName));
}