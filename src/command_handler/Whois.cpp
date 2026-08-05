#include "Command_handler.hpp"
#include "Channel.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

WhoisCommandHandler::WhoisCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

WhoisCommandHandler::~WhoisCommandHandler()
{
}

void WhoisCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() != 1)
        throw InvalidParametersException(_client.getNickname(), "WHOIS");

    std::string targetNick = params[0];
    Client *target = _server.searchNickname(targetNick);

    if (target == NULL)
        throw NoSuchNickException(_client.getNickname(), targetNick);

    _server.replyToClient(_client.getFd(),
        RPL_WHOISUSER(_client.getNickname(), target->getNickname(), target->getUsername(),
                      target->getHost(), target->getRealname()));

    _server.replyToClient(_client.getFd(),
        RPL_WHOISSERVER(_client.getNickname(), target->getNickname(), target->getServername()));

    std::string channelList;
    std::map<std::string, Channel*> allChannels = _server.getChannels();
    std::map<std::string, Channel*>::iterator it = allChannels.begin();
    while (it != allChannels.end())
    {
        if (it->second->isMember(target->getFd()))
        {
            if (!channelList.empty())
                channelList += " ";
            if (it->second->isOperator(target->getFd()))
                channelList += "@";
            channelList += it->first;
        }
        ++it;
    }

    if (!channelList.empty())
        _server.replyToClient(_client.getFd(),
            RPL_WHOISCHANNELS(_client.getNickname(), target->getNickname(), channelList));

    _server.replyToClient(_client.getFd(), RPL_ENDOFWHOIS(_client.getNickname(), target->getNickname()));
}