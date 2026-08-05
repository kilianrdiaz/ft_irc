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
        throw InvalidParametersException(_client.get_prefix(), "TOPIC");

    std::string channelName = params[0];

    Channel *channel = _server.getChannelByName(channelName);

    if (!channel)
        throw InvalidChannelException(_client.get_prefix(), channelName);

    if (!channel->isMember(_client.getFd()))
        throw NotInChannelException(_client.get_prefix(), channelName);

    if (params.size() == 1)
    {
        // Solo consulta, no cambio
        if (channel->getTopic().empty())
            _client.write(RPL_NOTOPIC(_client.get_prefix(), channelName));
        else
            _client.write(RPL_TOPIC(_client.get_prefix(), channelName, channel->getTopic()));
        return;
    }

    // Se quiere cambiar el topic
    if (channel->getTopicRestricted() && !channel->isOperator(_client.getFd()))
        throw NotPrivilegedException(_client.get_prefix(), channelName);

    std::string newTopic = params[1];
    channel->setTopic(newTopic);
    _client.write(RPL_TOPIC(_client.get_prefix(), channelName, newTopic));
}