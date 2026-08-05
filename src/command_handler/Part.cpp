#include "Command_handler.hpp"
#include "Channel.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

PartChannelCommandHandler::PartChannelCommandHandler(Server &server, Client &client) : AChannelCommandHandler(server, client)
{
}

PartChannelCommandHandler::~PartChannelCommandHandler()
{
}

void PartChannelCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() < 1 || params.size() > 2)
        throw InvalidParametersException(_client.get_prefix(), "PART");

    std::string channelName = params[0];

    Channel *channel = _server.getChannelByName(channelName);

    if (!channel)
        throw InvalidChannelException(_client.get_prefix(), channelName);

    if (!channel->isMember(_client.getFd()))
        throw NotInChannelException(_client.get_prefix(), channelName);
    std::string reason = params.size() > 1 ? params[1] : "";
    channel->broadcast(MSG_PART(_client.get_prefix(), channelName, reason), _client.getFd());
    channel->removeMember(_client.getFd());
    if (channel->memberCount() == 0)
        _server.removeChannel(channelName);
    _client.write(MSG_PART(_client.get_prefix(), channelName, reason));
}