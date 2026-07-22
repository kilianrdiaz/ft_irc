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

// duplicada en Mode.cpp
static int findMemberFdByNickname(Channel &channel, const std::string &nickname)
{
    std::map<int, ChannelMember> &members = channel.getMembers();
    std::map<int, ChannelMember>::iterator it = members.begin();

    while (it != members.end())
    {
        if (it->second.client->getNickname() == nickname)
            return it->first;
        it++;
    }
    return -1;
}

void KickChannelCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() < 2)
        throw InvalidParametersException(_client.getNickname(), "KICK");

    std::string channelName = params[0];
    std::string targetNick = params[1];
    std::string reason = (params.size() > 2) ? params[2] : "Kicked";

    std::map<std::string, Channel*> &channels = _server.getChannels();
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);

    if (it == channels.end())
        throw InvalidChannelException(_client.getNickname(), channelName);

    Channel *channel = it->second;

    if (!channel->isMember(_client.getFd()))
        throw NotInChannelException(_client.getNickname(), channelName);

    if (!channel->isOperator(_client.getFd()))
        throw NotPrivilegedException(_client.getNickname(), channelName);

    int targetFd = findMemberFdByNickname(*channel, targetNick);

    if (targetFd == -1)
        throw ChannelException(ERR_USERNOTINCHANNEL(_client.getNickname(), targetNick, channelName));

    channel->removeMember(targetFd);
    _client.write(MSG_KICK(_client.getNickname(), channelName, targetNick, reason));

    if (channel->memberCount() == 0)
    {
        delete channel;
        channels.erase(it);
    }
}