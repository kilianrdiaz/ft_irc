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
    std::map<std::string, Channel*> allChannels = _server.getChannels();

    _server.replyToClient(_client.getFd(), RPL_LISTSTART(_client.getNickname()));

    for (std::map<std::string, Channel*>::iterator it = allChannels.begin(); it != allChannels.end(); ++it)
    {
        if (!params.empty() && params[0] != it->first)
            continue;

        _server.replyToClient(_client.getFd(),
            RPL_LIST(_client.getNickname(), it->first, ::toStr(it->second->memberCount()), it->second->getTopic()));
    }

    _server.replyToClient(_client.getFd(), RPL_LISTEND(_client.getNickname()));
}