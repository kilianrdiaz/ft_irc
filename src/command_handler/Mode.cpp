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

void ModeCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() < 2)
        throw InvalidParametersException(_client.get_prefix(), "MODE");

    std::string channelName = params[0];
    std::string modeString = params[1];

    Channel *channel = _server.getChannelByName(channelName);
    if (!channel)
        throw InvalidChannelException(_client.get_prefix(), channelName);


    if (!channel->isMember(_client.getFd()))
        throw NotInChannelException(_client.get_prefix(), channelName);

    if (!channel->isOperator(_client.getFd()))
        throw NotPrivilegedException(_client.get_prefix(), channelName);

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
                        throw InvalidParametersException(_client.get_prefix(), "MODE");
                    channel->setKey(params[argIndex++]);
                }
                else
                    channel->setKey("");
                break;

            case 'o':
            {
                if (argIndex >= params.size())
                    throw InvalidParametersException(_client.get_prefix(), "MODE");

                std::string targetNick = params[argIndex++];
                int targetFd = _server.searchNickname(targetNick) ? _server.searchNickname(targetNick)->getFd() : -1;

                if (targetFd == -1)
                    throw ChannelException(ERR_USERNOTINCHANNEL(_client.get_prefix(), targetNick, channelName));

                channel->setOperator(targetFd, sign);
                break;
            }

            case 'l':
                if (sign)
                {
                    if (argIndex >= params.size())
                        throw InvalidParametersException(_client.get_prefix(), "MODE");

                    size_t limit;
                    if (!parseLimit(params[argIndex++], limit))
                        throw InvalidModeException(_client.get_prefix(), "l");
                    channel->setUserLimit(limit);
                }
                else
                    channel->setUserLimit(0);
                break;

            default:
                throw InvalidModeException(_client.get_prefix(), std::string(1, c));
        }
    }

    std::string argsJoined;
    for (size_t i = 2; i < params.size(); i++)
        argsJoined += (i > 2 ? " " : "") + params[i];

    _client.write(MSG_MODE(_client.get_prefix(), channelName, modeString, argsJoined));
}