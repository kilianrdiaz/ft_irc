#include "Command_handler.hpp"
#include "Channel.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

TopicCommandHandler::TopicCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
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

    std::map<std::string, Channel*> &channels = _server.getChannels();
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);

    if (it == channels.end())
        throw InvalidChannelException(_client.getNickname(), channelName);

    Channel *channel = it->second;

    if (!channel->isMember(_client.getFd()))
        throw NotInChannelException(_client.getNickname(), channelName);

    if (params.size() == 1)
    {
        // Solo consulta, no cambio
        if (channel->getTopic().empty())
            _client.write(RPL_NOTOPIC(_client.getNickname(), channelName));
        else
            _client.write(RPL_TOPIC(_client.getNickname(), channelName, channel->getTopic()));
        return;
    }

    // Se quiere cambiar el topic
    if (channel->getTopicRestricted() && !channel->isOperator(_client.getFd()))
        throw NotPrivilegedException(_client.getNickname(), channelName);

    std::string newTopic = params[1];
    channel->setTopic(newTopic);
    _client.write(RPL_TOPIC(_client.getNickname(), channelName, newTopic));
}