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
        throw InvalidParametersException(_client.getNickname(), "JOIN");   // CAMBIA: excepción

    std::string channelName = params[0];
    std::string providedKey = (params.size() > 1) ? params[1] : "";

    std::vector<std::string> channelNames = parseChannelList(channelName);
    for (size_t i = 0; i < channelNames.size(); i++)
    {
        std::string currentChannelName = channelNames[i];

        try
        {
            if (currentChannelName.empty() || currentChannelName[0] != '#')
                throw InvalidChannelException(_client.getNickname(), currentChannelName);   // CAMBIA: excepción

            Channel *channel = _server.getChannelByName(currentChannelName);

            if (!channel)
            {
                Channel *newChannel = new Channel(currentChannelName, providedKey);
                newChannel->addMember(&_client, 1);
                _server.addChannel(newChannel);
                _client.write(MSG_JOIN(_client.get_prefix(), currentChannelName));          // SE QUEDA: MSG_*
                _server.replyToClient(_client.getFd(),
                    RPL_NAMREPLY(_client.getNickname(), currentChannelName, "@"+_client.getNickname()));  // CAMBIA: RPL_*
                _server.replyToClient(_client.getFd(),
                    RPL_ENDOFNAMES(_client.getNickname(), currentChannelName));             // CAMBIA: RPL_*
                continue;
            }

            if (channel->isMember(_client.getFd()))
                throw AlreadyInChannelException(_client.getNickname(), currentChannelName);   // CAMBIA: excepción

            if (channel->getInviteOnly() && !channel->isInvited(_client.getFd()))
                throw InviteOnlyChannelException(_client.getNickname(), currentChannelName);  // CAMBIA: excepción

            if (channel->getKey() != "" && channel->getKey() != providedKey)
                throw BadChannelKeyException(_client.getNickname(), currentChannelName);      // CAMBIA: excepción

            if (channel->hasUserLimit() && channel->isFull())
                throw ChannelFullException(_client.getNickname(), currentChannelName);        // CAMBIA: excepción

            channel->addMember(&_client, 0);
            channel->removeInvite(_client.getFd());
            _client.write(MSG_JOIN(_client.get_prefix(), currentChannelName));               // SE QUEDA: MSG_*
            _server.replyToClient(_client.getFd(),
                RPL_NAMREPLY(_client.getNickname(), currentChannelName, channel->getMemberList(_server)));  // CAMBIA: RPL_*
            _server.replyToClient(_client.getFd(),
                RPL_ENDOFNAMES(_client.getNickname(), currentChannelName));                  // CAMBIA: RPL_*
            channel->broadcast(MSG_JOIN(_client.get_prefix(), currentChannelName), _client.getFd());  // SE QUEDA: MSG_*
        }
        catch (CommandException &e)
        {
            _server.replyToClient(_client.getFd(), e.what());
        }
    }
}