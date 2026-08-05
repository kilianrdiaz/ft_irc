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
        throw InvalidParametersException(_client.get_prefix(), "JOIN");

    std::string channelName = params[0];
    std::string providedKey = (params.size() > 1) ? params[1] : "";

    std::vector<std::string> channelNames = parseChannelList(channelName);
    for (size_t i = 0; i < channelNames.size(); i++)
    {
        std::string currentChannelName = channelNames[i];

        try
        {
            if (currentChannelName.empty() || currentChannelName[0] != '#')
                throw InvalidChannelException(_client.get_prefix(), currentChannelName);

            Channel *channel = _server.getChannelByName(currentChannelName);

            if (!channel)
            {
                // Canal no existe -> crearlo (ya sabemos que el nombre es válido, empieza por #)
                Channel *newChannel = new Channel(currentChannelName, providedKey);
                newChannel->addMember(&_client, 1);
                _server.addChannel(newChannel);
                _client.write(MSG_JOIN(_client.get_prefix(), currentChannelName));
                _server.replyToClient(_client.getFd(),
                    RPL_NAMREPLY(_client.get_prefix(), currentChannelName, "@"+_client.getNickname()));
                _server.replyToClient(_client.getFd(),
                    RPL_ENDOFNAMES(_client.get_prefix(), currentChannelName));
                continue;
            }

            if (channel->isMember(_client.getFd()))
                throw AlreadyInChannelException(_client.get_prefix(), currentChannelName);

            if (channel->getInviteOnly() && !channel->isInvited(_client.getFd()))
                throw InviteOnlyChannelException(_client.get_prefix(), currentChannelName);

            if (channel->getKey() != "" && channel->getKey() != providedKey)
                throw BadChannelKeyException(_client.get_prefix(), currentChannelName);

            if (channel->hasUserLimit() && channel->isFull())
                throw ChannelFullException(_client.get_prefix(), currentChannelName);

            channel->addMember(&_client, 0);
            channel->removeInvite(_client.getFd());
            _client.write(MSG_JOIN(_client.get_prefix(), currentChannelName));
            _server.replyToClient(_client.getFd(),
                RPL_NAMREPLY(_client.get_prefix(), currentChannelName, channel->getMemberList(_server)));
            _server.replyToClient(_client.getFd(),
                RPL_ENDOFNAMES(_client.get_prefix(), currentChannelName));
            channel->broadcast(MSG_JOIN(_client.get_prefix(), currentChannelName), _client.getFd());
        }
        catch (CommandException &e)
        {
            _server.replyToClient(_client.getFd(), e.what());
        }
    }
}