#include "Channel.hpp"
#include "Command_handler.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

ListChannelCommandHandler::ListChannelCommandHandler(Server &server,
	Client &client) : AChannelCommandHandler(server, client)
{
}

ListChannelCommandHandler::~ListChannelCommandHandler()
{
}

void ListChannelCommandHandler::execute(const std::vector<std::string> &params)
{
	if (params.size() == 0)
	{
		std::string channelList = _server.listChannels(_client.getFd());
		_client.write(MSG_LIST(_client.get_prefix(), channelList));
		return;
	}
	std::string channelName = params[0];
	Channel *channel = _server.getChannelByName(channelName);

	if (!channel)
		throw InvalidChannelException(_client.getNickname(), channelName);
	if (!channel->isMember(_client.getFd()))
		throw NotInChannelException(_client.getNickname(), channelName);

	_server.replyToClient(_client.getFd(), RPL_NAMREPLY(_client.getNickname(),
			channelName, channel->getMemberList(_server)));
	_server.replyToClient(_client.getFd(), RPL_ENDOFNAMES(_client.getNickname(),
			channelName));
}