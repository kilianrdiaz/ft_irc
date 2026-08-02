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
        throw InvalidParametersException(_client.getNickname(), "PART");

    std::string channelName = params[0];

    Channel *channel = _server.getChannelByName(channelName);

    if (!channel)
        throw InvalidChannelException(_client.getNickname(), channelName);

    if (!channel->isMember(_client.getFd()))
        throw NotInChannelException(_client.getNickname(), channelName);

    channel->removeMember(_client.getFd());
    std::string reason = params.size() > 1 ? params[1] : "";
    _client.write(MSG_PART(_client.getNickname(), channelName, reason));

    if (channel->memberCount() == 0)
    {
        _server.removeChannel(channelName);
    }
}