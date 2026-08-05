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

    Channel *channel = _server.getChannelByName(channelName);

    if (!channel)
        throw InvalidChannelException(_client.getNickname(), channelName);  

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

    _server.replyToClient(_client.getFd(), RPL_INVITING(_client.getNickname(), targetNick, channelName));
    targetClient->write(MSG_INVITE(_client.get_prefix(), targetNick, channelName));
}