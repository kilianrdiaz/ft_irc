#include "Channel.hpp"
#include "Command_handler.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

ListCommandHandler::ListCommandHandler(Server &server,
	Client &client) : AChannelCommandHandler(server, client)
{
}

ListCommandHandler::~ListCommandHandler()
{
}

void ListCommandHandler::execute(const std::vector<std::string> &params)
{
	(void)params;
	std::string channelList = _server.listChannels(_client.getFd());
	_client.write(MSG_LIST(_client.get_prefix(), channelList));
}