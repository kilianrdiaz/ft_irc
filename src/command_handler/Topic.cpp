#include "Command_handler.hpp"
#include "Channel.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

TopicCommandHandler::TopicCommandHandler(Server &server, Client &client) : AChannelCommandHandler(server, client)
{
}

TopicCommandHandler::~TopicCommandHandler()
{
}

void TopicCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.empty() || params.size() > 2)
        throw InvalidParametersException(_client.getNickname(), "TOPIC");

    std::string channelName = params[0];

    Channel *channel = _server.getChannelByName(channelName);

    if (!channel)
        throw InvalidChannelException(_client.getNickname(), channelName);

    if (!channel->isMember(_client.getFd()))
        throw NotInChannelException(_client.getNickname(), channelName);

    if (params.size() == 1)
    {
        if (channel->getTopic().empty())
            _server.replyToClient(_client.getFd(), RPL_NOTOPIC(_client.getNickname(), channelName));
        else
            _server.replyToClient(_client.getFd(), RPL_TOPIC(_client.getNickname(), channelName, channel->getTopic()));
        return;
    }

    if (channel->getTopicRestricted() && !channel->isOperator(_client.getFd()))
        throw NotPrivilegedException(_client.getNickname(), channelName);

    std::string newTopic = params[1];
    channel->setTopic(newTopic);

    channel->broadcast(MSG_TOPIC(_client.get_prefix(), channelName, newTopic), -1);
}