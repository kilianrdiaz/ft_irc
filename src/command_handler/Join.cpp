#include "Command_handler.hpp"
#include "Channel.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

JoinChannelCommandHandler::JoinChannelCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
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

    std::map<std::string, Channel*> &channels = _server.getChannels();
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);

    if (it == channels.end())
    {
        Channel *newChannel = new Channel(channelName, providedKey);
        newChannel->addMember(&_client, true);
        channels[channelName] = newChannel;
    }
    else
    {
        Channel *channel = it->second;

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

    _client.write(RPL_JOIN(_client.getNickname(), channelName));
}