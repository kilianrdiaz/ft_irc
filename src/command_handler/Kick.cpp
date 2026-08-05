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
        throw InvalidParametersException(_client.getNickname(), "KICK");   // CAMBIA

    std::string channelName = params[0];
    std::string targetNick = params[1];
    std::string reason = (params.size() > 2) ? params[2] : "Kicked";

    Channel *channel = _server.getChannelByName(channelName);
    Client *target = _server.searchNickname(targetNick);

    if (!channel)
        throw InvalidChannelException(_client.getNickname(), channelName);   // CAMBIA

    if (!channel->isOperator(_client.getFd()))
        throw NotPrivilegedException(_client.getNickname(), channelName);    // CAMBIA
    if (!target)
        throw NoSuchNickException(_client.getNickname(), targetNick);        // CAMBIA
    if (!channel->isMember(target->getFd()))
        throw TargetNotInChannelException(_client.getNickname(), targetNick, channelName);   // CAMBIA


    int targetFd = _server.searchNickname(targetNick) ? _server.searchNickname(targetNick)->getFd() : -1;

    if (targetFd == -1)
        throw ChannelException(ERR_USERNOTINCHANNEL(_client.getNickname(), target->getNickname(), channelName));   // CAMBIA (las dos)

    std::string kickMsg = MSG_KICK(_client.get_prefix(), channelName, targetNick, reason);   // SE QUEDA: MSG_*

    channel->broadcast(kickMsg, _client.getFd());
    channel->removeMember(targetFd);
    if (channel->memberCount() == 0)
        _server.removeChannel(channelName);
}