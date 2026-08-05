#include "Command_handler.hpp"
#include "Channel.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

KickChannelCommandHandler::KickChannelCommandHandler(Server &server, Client &client) : AChannelCommandHandler(server, client)
{
}

KickChannelCommandHandler::~KickChannelCommandHandler()
{
}

void KickChannelCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() < 2)
        throw InvalidParametersException(_client.get_prefix(), "KICK");

    std::string channelName = params[0];
    std::string targetNick = params[1];
    std::string reason = (params.size() > 2) ? params[2] : "Kicked";

    Channel *channel = _server.getChannelByName(channelName);
    Client *target = _server.searchNickname(targetNick);

    if (!channel)
        throw InvalidChannelException(_client.get_prefix(), channelName);

    if (!channel->isOperator(_client.getFd()))
        throw NotPrivilegedException(_client.get_prefix(), channelName);
    if (!target)
        throw NoSuchNickException(_client.get_prefix(), targetNick);
    if (!channel->isMember(target->getFd()))
        throw TargetNotInChannelException(_client.get_prefix(), targetNick, channelName);


    int targetFd = _server.searchNickname(targetNick) ? _server.searchNickname(targetNick)->getFd() : -1;

    if (targetFd == -1)
        throw ChannelException(ERR_USERNOTINCHANNEL(_client.get_prefix(), target->get_prefix(), channelName));

    std::string kickMsg = MSG_KICK(_client.get_prefix(), channelName, targetNick, reason);

    channel->broadcast(kickMsg, _client.getFd());
    channel->removeMember(targetFd);
    if (channel->memberCount() == 0)
        _server.removeChannel(channelName);
}