#include "Command_handler.hpp"
#include "Channel.hpp"
#include "command_excepts.hpp"
#include "response.hpp"
#include <cstdlib>

ModeCommandHandler::ModeCommandHandler(Server &server, Client &client) : AChannelCommandHandler(server, client)
{
}

ModeCommandHandler::~ModeCommandHandler()
{
}

static bool parseLimit(const std::string &str, size_t &out)
{
    if (str.empty())
        return false;

    for (size_t i = 0; i < str.size(); i++)
    {
        if (!isdigit(str[i]))
            return false;
    }

    out = static_cast<size_t>(atol(str.c_str()));
    return true;
}

static int findMemberFdByNickname(Channel &channel, const std::string &nickname)
{
    std::map<int, ChannelMember> &members = channel.getMembers();
    std::map<int, ChannelMember>::iterator it = members.begin();

    while (it != members.end())
    {
        if (it->second.client->getNickname() == nickname)
            return it->first;
        it++;
    }
    return -1;
}

void ModeCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() < 2)
        throw InvalidParametersException(_client.getNickname(), "MODE");

    std::string channelName = params[0];
    std::string modeString = params[1];

    std::map<std::string, Channel*> &channels = _server.getChannels();
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);

    if (it == channels.end())
        throw InvalidChannelException(_client.getNickname(), channelName);

    Channel *channel = it->second;

    if (!channel->isMember(_client.getFd()))
        throw NotInChannelException(_client.getNickname(), channelName);

    if (!channel->isOperator(_client.getFd()))
        throw NotPrivilegedException(_client.getNickname(), channelName);

    size_t argIndex = 2;
    bool sign = true;

    for (size_t i = 0; i < modeString.size(); i++)
    {
        char c = modeString[i];

        if (c == '+')
        {
            sign = true;
            continue;
        }
        if (c == '-')
        {
            sign = false;
            continue;
        }

        switch (c)
        {
            case 'i':
                channel->setInviteOnly(sign);
                break;

            case 't':
                channel->setTopicRestricted(sign);
                break;

            case 'k':
                if (sign)
                {
                    if (argIndex >= params.size())
                        throw InvalidParametersException(_client.getNickname(), "MODE");
                    channel->setKey(params[argIndex++]);
                }
                else
                    channel->setKey("");
                break;

            case 'o':
            {
                if (argIndex >= params.size())
                    throw InvalidParametersException(_client.getNickname(), "MODE");

                std::string targetNick = params[argIndex++];
                int targetFd = findMemberFdByNickname(*channel, targetNick);

                if (targetFd == -1)
                    throw ChannelException(ERR_USERNOTINCHANNEL(_client.getNickname(), targetNick, channelName));

                channel->setOperator(targetFd, sign);
                break;
            }

            case 'l':
                if (sign)
                {
                    if (argIndex >= params.size())
                        throw InvalidParametersException(_client.getNickname(), "MODE");

                    size_t limit;
                    if (!parseLimit(params[argIndex++], limit))
                        throw InvalidModeException(_client.getNickname(), "l");
                    channel->setUserLimit(limit);
                }
                else
                    channel->setUserLimit(0);
                break;

            default:
                throw InvalidModeException(_client.getNickname(), std::string(1, c));
        }
    }

    std::string argsJoined;
    for (size_t i = 2; i < params.size(); i++)
        argsJoined += (i > 2 ? " " : "") + params[i];

    _client.write(MSG_MODE(_client.getNickname(), channelName, modeString, argsJoined));
}