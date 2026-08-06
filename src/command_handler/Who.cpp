#include "Channel.hpp"
#include "Command_handler.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

WhoChannelCommandHandler::WhoChannelCommandHandler(Server &server,
	Client &client) : AChannelCommandHandler(server, client)
{
}

WhoChannelCommandHandler::~WhoChannelCommandHandler()
{
}

void WhoChannelCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() == 0)
        throw InvalidParametersException(_client.getNickname(), "WHO");

    std::string channelName = params[0];
    Channel *channel = _server.getChannelByName(channelName);

    if (!channel)
        throw InvalidChannelException(_client.getNickname(), channelName);
    if (!channel->isMember(_client.getFd()))
        throw NotInChannelException(_client.getNickname(), channelName);

    std::map<int, bool> &members = channel->getMembers();
    for (std::map<int, bool>::iterator it = members.begin(); it != members.end(); ++it)
    {
        Client *member = _server.getClientByFd(it->first);
        if (member == NULL)
            continue;

        std::string flags = "H";
        if (channel->isOperator(member->getFd()))
            flags += "@";

        _server.replyToClient(_client.getFd(),
            RPL_WHOREPLY(_client.getNickname(), channelName, member->getUsername(),
                        member->getHost(), member->getServername(), member->getNickname(),
                        flags, member->getRealname()));
    }

    _server.replyToClient(_client.getFd(), RPL_ENDOFWHO(_client.getNickname(), channelName));
}