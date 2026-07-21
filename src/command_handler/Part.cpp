#include "Command_handler.hpp"
#include "Channel.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

PartChannelCommandHandler::PartChannelCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

PartChannelCommandHandler::~PartChannelCommandHandler()
{
}

void PartChannelCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() != 1)
        throw InvalidParametersException(_client.getNickname(), "PART");

    std::string channelName = params[0];

    std::map<std::string, Channel*> &channels = _server.getChannels();
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);

    if (it == channels.end())
        throw InvalidChannelException(_client.getNickname(), channelName);

    Channel *channel = it->second;

    if (!channel->isMember(_client.getFd()))
        throw NotInChannelException(_client.getNickname(), channelName);

    channel->removeMember(_client.getFd());
    _client.write(RPL_PART(_client.getNickname(), channelName));

    if (channel->memberCount() == 0)
    {
        delete channel;
        channels.erase(it);
    }
}