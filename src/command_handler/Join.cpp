#include "Command_handler.hpp"
#include "Channel.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

JoinChannelCommandHandler::JoinChannelCommandHandler(Server &server, Client &client) : AChannelCommandHandler(server, client)
{
}

JoinChannelCommandHandler::~JoinChannelCommandHandler()
{
}

void JoinChannelCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.empty() || params.size() > 2)
        throw InvalidParametersException(_client.getNickname(), "JOIN");

    std::string channelName = params[0];

    if (channelName.empty() || channelName[0] != '#')
        throw InvalidChannelException(_client.getNickname(), channelName);

    std::string providedKey = (params.size() > 1) ? params[1] : "";

    Channel *channel = this->getChannelByName(channelName);
    if (!channel)
    {
        Channel *newChannel = new Channel(channelName, providedKey);
        newChannel->addMember(&_client, true);
        _server.getChannels()[channelName] = newChannel;
    }
    else
    {
        if (channel->isMember(_client.getFd()))
            return;

        if (channel->getInviteOnly() && !channel->isInvited(_client.getFd()))
            throw InviteOnlyChannelException(_client.getNickname(), channelName);

        if (channel->hasKey() && channel->getKey() != providedKey)
            throw BadChannelKeyException(_client.getNickname(), channelName);

        if (channel->isFull())
            throw ChannelFullException(_client.getNickname(), channelName);

        channel->addMember(&_client, false);
        channel->removeInvite(_client.getFd());
    }

    _client.write(MSG_JOIN(_client.getNickname(), channelName));
}