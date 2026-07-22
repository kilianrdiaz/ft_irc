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
        throw InvalidParametersException(_client.getNickname(), "JOIN");

    std::string channelName = params[0];
    std::string providedKey = (params.size() > 1) ? params[1] : "";

    std::vector<std::string> channelNames = parseChannelList(channelName);
    for (size_t i = 0; i < channelNames.size(); i++)
    {
        std::string currentChannelName = channelNames[i];

        try
        {
            if (currentChannelName.empty() || currentChannelName[0] != '#')
                throw InvalidChannelException(_client.getNickname(), currentChannelName);

            std::map<std::string, Channel*>::iterator it =
                _server.getChannels().find(currentChannelName);

            if (it == _server.getChannels().end())
            {
                // Canal no existe -> crearlo (ya sabemos que el nombre es válido, empieza por #)
                Channel *newChannel = new Channel(currentChannelName, providedKey);
                newChannel->addMember(&_client, 1);
                _server.getChannels()[currentChannelName] = newChannel;
                _client.write(MSG_JOIN(_client.getNickname(), currentChannelName));
                _server.replyToClient(_client.getFd(),
                    RPL_NAMREPLY(_client.getNickname(), currentChannelName, _client.getNickname()));
                _server.replyToClient(_client.getFd(),
                    RPL_ENDOFNAMES(_client.getNickname(), currentChannelName));
                continue;
            }

            Channel *channel = it->second;

            if (channel->isMember(_client.getFd()))
                throw AlreadyInChannelException(_client.getNickname(), currentChannelName);
            if (channel->getInviteOnly())
                throw InviteOnlyChannelException(_client.getNickname(), currentChannelName);
            else if (channel->getKey() != "" && channel->getKey() != providedKey)
                throw BadChannelKeyException(_client.getNickname(), currentChannelName);
            if (channel->hasUserLimit() && channel->isFull())
                throw ChannelFullException(_client.getNickname(), currentChannelName);

            channel->addMember(&_client, 0);
            _client.write(MSG_JOIN(_client.getNickname(), currentChannelName));

            std::string memberList;
            {
                const std::map<int, ChannelMember> &members = channel->getMembers();
                size_t j = 0;
                for (std::map<int, ChannelMember>::const_iterator mit = members.begin();
                     mit != members.end(); ++mit)
                {
                    if (j) memberList += " ";
                    memberList += mit->second.client->getNickname();
                    j++;
                }
            }
            _server.replyToClient(_client.getFd(),
                RPL_NAMREPLY(_client.getNickname(), currentChannelName, memberList));
            _server.replyToClient(_client.getFd(),
                RPL_ENDOFNAMES(_client.getNickname(), currentChannelName));
        }
        catch (CommandException &e)
        {
            _server.replyToClient(_client.getFd(), e.what());
        }
    }
}